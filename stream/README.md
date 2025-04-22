# Convert Stream from .h264 to .mp4
ffmpeg -i [stream_name].h264 -c:v copy -bsf:v h264_mp4toannexb -c:a copy [file_name].mp4
