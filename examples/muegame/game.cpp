#include <iostream>
#include <utility>
#include <format>
#include "VHInclude.h"
#include "VEInclude.h"


class MyGame : public vve::System {

    enum class State : int {
        STATE_RUNNING,
        STATE_COLLISION
    };

    const float c_max_time = 35.0f;
    const int c_field_size = 50;
    const int c_number_obstacles = 10;
    const float c_min_distance = 10.0f; // Minimum distance between camera and obstacle cubes

    int nextRandom() {
        return rand() % (c_field_size) - c_field_size / 2;
    }

public:
    MyGame(vve::Engine& engine) : vve::System("MyGame", engine) {

        m_engine.RegisterCallbacks({
                                           {this, 0, "LOAD_LEVEL", [this](Message& message) { return OnLoadLevel(message); }},
                                           {this, 10000, "UPDATE", [this](Message& message) { return OnUpdate(message); }},
                                           {this, -10000, "RECORD_NEXT_FRAME", [this](Message& message) { return OnRecordNextFrame(message); }}
                                   });
    };

    ~MyGame() {};

    void GetCamera() {
        if (m_cameraHandle.IsValid() == false) {
            auto [handle, camera, parent] = *m_registry.GetView<vecs::Handle, vve::Camera&, vve::ParentHandle>().begin();
            m_cameraHandle = handle;
            m_cameraNodeHandle = parent;
        };
    }

    inline static std::string plane_obj { "assets/test/plane/plane_t_n_s.obj" };
    inline static std::string plane_mesh { "assets/test/plane/plane_t_n_s.obj/plane" };
    inline static std::string plane_txt { "assets/test/plane/grass.jpg" };

    inline static std::string cube_obj { "assets/test/crate0/cube.obj" };

    bool OnLoadLevel(Message message) {
        auto msg = message.template GetData<vve::System::MsgLoadLevel>();
        std::cout << "Loading level: " << msg.m_level << std::endl;
        std::string level = std::string("Level: ") + msg.m_level;

        m_engine.SendMsg(MsgSceneLoad{ vve::Filename{plane_obj}, aiProcess_FlipWindingOrder });

        auto m_handlePlane = m_registry.Insert(
                vve::Position{ {0.0f, 0.0f, 0.0f} },
                vve::Rotation{ mat3_t { glm::rotate(glm::mat4(1.0f), 3.14152f / 2.0f, glm::vec3(1.0f, 0.0f, 0.0f)) } },
                vve::Scale{ vec3_t{1000.0f, 1000.0f, 1000.0f} },
                vve::MeshName{ plane_mesh },
                vve::TextureName{ plane_txt },
                vve::UVScale{ {1000.0f, 1000.0f} }
        );

        m_engine.SendMsg(MsgObjectCreate{ vve::ObjectHandle(m_handlePlane), vve::ParentHandle{} });

        for (int i = 0; i < c_number_obstacles; ++i) {
            vec2_t obstaclePos = vec2_t{ static_cast<float>(nextRandom()), static_cast<float>(nextRandom()) };
            auto handleObstacle = m_registry.Insert(
                    vve::Position{ {obstaclePos.x, obstaclePos.y, 0.5f} },
                    vve::Rotation{ mat3_t{1.0f} },
                    vve::Scale{ vec3_t{1.0f} }
            );

            m_engine.SendMsg(MsgSceneCreate{ vve::ObjectHandle(handleObstacle), vve::ParentHandle{}, vve::Filename{cube_obj}, aiProcess_FlipWindingOrder });
            m_obstacles.push_back(handleObstacle);
        }

        GetCamera();
        m_registry.Get<vve::Rotation&>(m_cameraHandle)() = mat3_t{ glm::rotate(mat4_t{1.0f}, 3.14152f / 2.0f, vec3_t{1.0f, 0.0f, 0.0f}) };

        m_engine.SendMsg(MsgPlaySound{ vve::Filename{"assets/sounds/ophelia.wav"}, 2 });

        return false;
    };

    bool OnUpdate(Message& message) {
        auto msg = message.template GetData<vve::System::MsgUpdate>();
        m_time_elapsed += msg.m_dt;

        // Send FRAME_END each update
        m_engine.SendMsg(MsgFrameEndFFMPEG{msg.m_dt});

        auto posCameraRef = m_registry.Get<vve::Position&>(m_cameraNodeHandle);
        auto& posCamera = posCameraRef();
        posCamera.z = 0.5f;

        if (m_state == State::STATE_RUNNING) {
            for (const auto& obs : m_obstacles) {
                auto posObsRef = m_registry.Get<vve::Position&>(obs);
                auto& posObs = posObsRef();
                vec2_t dir = vec2_t{ posCamera.x, posCamera.y } - vec2_t{ posObs.x, posObs.y };
                dir = glm::normalize(dir);
                posObs.x += dir.x * msg.m_dt * 2.0f;
                posObs.y += dir.y * msg.m_dt * 2.0f;

                float dist = glm::length(vec2_t{ posCamera.x, posCamera.y } - vec2_t{ posObs.x, posObs.y });
                if (dist < 1.5f) {
                    std::cout << "Collision with obstacle!" << std::endl;
                    m_state = State::STATE_COLLISION;
                    m_engine.SendMsg(MsgPlaySound{ vve::Filename{"assets/sounds/explosion.wav"}, 1 });
                    return false;
                }
            }
        }

        return false;
    }

    bool OnRecordNextFrame(Message message) {
        if (m_state == State::STATE_RUNNING) {
            ImGui::Begin("Game State");
            char buffer[100];
            std::snprintf(buffer, 100, "Time Elapsed: %.2f s", m_time_elapsed);
            ImGui::TextUnformatted(buffer);
            ImGui::End();
        }

        if (m_state == State::STATE_COLLISION) {
            ImGui::Begin("Game State");
            ImGui::TextUnformatted("Game Over! Try Again.");
            if (ImGui::Button("Restart")) {
                ResetGame();
            }
            ImGui::End();
        }

        return false;
    }

    void ResetGame() {
        m_state = State::STATE_RUNNING;
        m_time_elapsed = 0.0f;

        m_registry.Get<vve::Position&>(m_cameraNodeHandle)() = {0.0f, 0.0f, 0.5f};

        for (auto& obstacle : m_obstacles) {
            vec2_t obstaclePos = vec2_t{ static_cast<float>(nextRandom()), static_cast<float>(nextRandom()) };
            m_registry.Get<vve::Position&>(obstacle)() = {obstaclePos.x, obstaclePos.y, 0.5f};
        }

        m_engine.SendMsg(MsgPlaySound{ vve::Filename{"assets/sounds/ophelia.wav"}, 2 });
    }



private:
    State m_state = State::STATE_RUNNING;
    float m_time_elapsed = 0.0f;
    vecs::Handle m_handlePlane{};
    std::vector<vecs::Handle> m_obstacles;
    vecs::Handle m_cameraHandle{};
    vecs::Handle m_cameraNodeHandle{};

    std::vector<AVFrame*> m_videoFrames;
    int m_videoWidth = 1280;
    int m_videoHeight = 720;
    int m_fps = 30;
};

int main() {
    vve::Engine engine("My Engine");
    MyGame mygui{ engine };
    engine.Run();
    return 0;
}
