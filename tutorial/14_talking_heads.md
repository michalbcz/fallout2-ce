# Tutorial 14: Creating a Talking Head

Talking heads (the animated 3D portraits with voice acting during dialogue) are one of Fallout's most iconic features. Creating a new one is an involved process requiring visual art, audio, and lip-syncing data.

## 1. The Art (.FRM)

A talking head is essentially a high-resolution, mostly static image, combined with animation frames for the mouth and eyes.

**The Modern Workflow:**
1. **Model & Render:** Create a bust of your character in a 3D program like Blender. Render a base idle frame. Then, render the different phoneme (mouth shape) frames for speaking.
2. **Convert to FRM:** Use a tool like **Frame Animator** to convert your rendered frames into a `.FRM` file. The engine uses palette indices to handle lighting, so ensure your render matches the 8-bit Fallout palette.
3. Save the resulting file to `data/art/heads/MYHEAD.FRM`.

## 2. Registering the Head (`heads.lst`)

The engine must be aware of the new art asset.
Open `data/art/heads/heads.lst` and add your file name (without extension) to the list.
```text
...
MARCUS
MYHEAD
```
*Note the line number you added it on; this is the Head ID.*

## 3. The Audio and Lip Sync (.LIP files)

When a talking head speaks, it plays an audio file and animates the mouth to match the syllables. The animation timing is controlled by a `.LIP` file.

**The Workflow:**
1. **Record Audio:** Record the voice line. Save it as an 11kHz or 22kHz `.WAV` file.
2. **Audio Conversion:** Fallout uses the `.ACM` format. You will need an ACM encoder (like **ndrw's ACM Tool** or **Wav2Acm**) to convert the `.WAV` into a `.ACM` or `.SND` file. Place it in `data/sound/speech/`.
3. **Lip Syncing:** You must generate a `.LIP` file for the audio so the engine knows when to open and close the mouth.
   - Historically, this was done manually or with Black Isle's internal tools.
   - **Modern Tool:** The community tool **LipSync/AutoLIP** can automatically analyze the audio waveform of your `.WAV` file and generate a `.LIP` file.
4. Place the `.LIP` file in the same directory, with the identical filename as your audio.

## 4. Script Integration

Now you must tell the script to use the talking head.

In your QuickJS dialogue script (or SSL script), when you initiate dialogue with `startGDialog()`, you pass the Head ID you noted from `heads.lst`.

```javascript
export function talk_p_proc() {
    let self = fallout.getSelf();
    let headID = 15; // Assume MYHEAD was line 15 in heads.lst

    // startGDialog(msgFileId, object, headId, backgroundId, reaction)
    fallout.startGDialog(1000, self, headID, -1, -1);

    fallout.gsayStart();
    // When gsayReply is called with a line that has a matching .ACM/.LIP file,
    // the head will animate and the voice will play!
    fallout.gsayReply(1000, 101);
    fallout.giqOption(4, 1000, 102, node_accept, 50);

    fallout.gsayEnd();
    fallout.endDialog();
}
```

By ensuring the dialogue message ID (101) maps to the correctly named audio file (according to the naming conventions in your `dialog.msg`), the engine handles the playback and animation automatically.
