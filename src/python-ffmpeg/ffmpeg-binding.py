'''
* TO BE DONE:
    -Ensure that audio works correctly.
'''
import ffmpeg
import os

def main():
    dataDir = './data/'
    preFormatVideoDir = 'video_input_dump/'
    postFormatVideoDir = 'converted_mp4_files/'
    
    if len(os.listdir(dataDir + preFormatVideoDir)) == 0:
        return
    
    #Converts given videos to mp4 (regardless of format)
    # WORKING
    for i in os.listdir(dataDir + preFormatVideoDir):
        # Pulling audio and video from input video (ensuring formatting)
        source = dataDir + preFormatVideoDir + i 
        dest = dataDir + postFormatVideoDir + i.split('.')[0] + '.mp4'
        video_stream = ffmpeg.input(source).video
        video_stream = ffmpeg.filter(video_stream, 'fps', fps=25, round='up')
        audio_stream = ffmpeg.input(source).audio
        # Setting up output streams with correct destination
        stream = ffmpeg.output(video_stream, audio_stream, dest)
        # Running command
        stream.overwrite_output().run()

    
    # Generating valid list for ffmpeg to use to concatenate videos
    # WORKING
    files = []
    for i in os.listdir(dataDir + postFormatVideoDir):
        files.append(f"file '{(postFormatVideoDir + i)}'")
    with open(dataDir + 'mp4list', 'w') as f:
        f.write('\n'.join(files))
    
    # Didn't feel like writing this one-liner using ffmpeg-python syntax, this format-string will do    
    # WORKING
    os.system(f"ffmpeg -y -fflags +igndts -f concat -safe 0 -i {dataDir + 'mp4list'} {dataDir + 'mergedVid.mp4'}")
    
    
        

if __name__ == '__main__':
    main()