import beyond.Reaper
import beyond.Screen
import os.path
from time import sleep
##from sws_python import *

# script inspired by post found at:  http://forum.cockos.com/showthread.php?t=128086
sheetMusic = 'Z;/CS410/finalOutput.txt'

@ProgramStart
def Main():

    # waits for the MIDI command list to appear at the expected location
    while os.path.exists(sheetMusic) == False:
        sleep(1)

    Reaper.OnConnectExecute = "from sws_python import *"
    with Reaper as r:
        # Determining how many milliseconds are in a MIDI tick from current tempo
        midiTicksPerQuarterNote = int(r.SNM_GetIntConfigVar("miditicksperbeat", -1))
        millisecondsPerMidiTick = 60000/(midiTicksPerQuarterNote * 120)

        maxTime = 0
        with open(sheetMusic,'r') as f:
            for line in f:
                time = 0

                # Deriving note information from line
                lineSplit = line.split(',')
                note = int(lineSplit[0])
                velocity = int(lineSplit[1])
                channel = int(lineSplit[2])
                channel = channel
                duration = int(lineSplit[3])
                time = time + duration
                if(duration < 100):
                    duration = 100
                    velocity = 127
                else:
                    velocity = 5
                duration = int(round(duration/millisecondsPerMidiTick))
                trackNum = int(lineSplit[4])
                startTime = int(lineSplit[5])
                time = time + startTime
                startTime = int(round(startTime/millisecondsPerMidiTick))

                # Determining what the length of the track item should be
                # based on when the last note stops
                if(time > maxTime):
                    maxTime = time

                # gets track and prepares MIDI take
                mediaTrack = r.GetTrack(0, trackNum)
                mediaItem = r.GetTrackMediaItem (mediaTrack, 0)
                r.SetMediaItemInfo_Value(mediaItem, "B_LOOPSRC", False)
                r.SetMediaItemInfo_Value(mediaItem, "D_LENGTH", 3600)
                take = r.GetTake(mediaItem, 0)
                allocated = r.FNG_AllocMidiTake(take)

                # writing MIDI note to track
                midiNote = r.FNG_AddMidiNote(allocated)
                r.FNG_SetMidiNoteIntProperty(midiNote, "CHANNEL", channel)
                r.FNG_SetMidiNoteIntProperty(midiNote, "VELOCITY", velocity)
                r.FNG_SetMidiNoteIntProperty(midiNote, "POSITION", startTime)
                r.FNG_SetMidiNoteIntProperty(midiNote, "PITCH", note)
                r.FNG_SetMidiNoteIntProperty(midiNote, "LENGTH", duration)

                r.FNG_FreeMidiTake(allocated)

        # alters the length of the MIDI tracks so that they are no longer than they need to be
        numTracks = r.GetNumTracks()
        for trackNum in range(0, numTracks):
            mediaTrack = r.GetTrack(0, trackNum)
            mediaItem = r.GetTrackMediaItem (mediaTrack, 0)
            r.SetMediaItemInfo_Value(mediaItem, "D_LENGTH", maxTime/1000)
