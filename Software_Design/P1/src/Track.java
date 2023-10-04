package ej3;

public class Track {

    private String position, recording, artist, title;
    private int duration;

    public String getPosition()
    {
        return this.position;
    }

    public String getRecording() {
        return this.recording;
    }

    public String getArtist() {
        return this.artist;
    }

    public String getTitle() {
        return this.title;
    }

    public int getDuration() {
        return this.duration;
    }

    public void setPosition(String position) {
        this.position = position;
    }

    public void setRecording(String recording) {
        this.recording = recording;
    }

    public void setArtist(String artist) {
        this.artist = artist;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public void setDuration(int duration) {
        this.duration = duration;
    }

    public Track(String position, String recording, String artist, String title, int duration) {
        this.position = position;
        this.recording = recording;
        this.artist = artist;
        this.title = title;
        this.duration = duration;
    }

    @Override
    public boolean equals(Object track)
    {
        //verifies if the classes are equal
        if (track.getClass() != Track.class)
            return false;

        //confirms that the variable we are using has class Release
        Track paramTrack = (Track) track;

        //verifies if the list sizes are equal
        if (this.recording.length() != paramTrack.recording.length())
            return false;

        for (int i = 0; i < this.recording.length(); i++)
        {
            if (this.recording.charAt(i) != paramTrack.recording.charAt(i))
                return false;
        }

        return true;
    }

    /*knowing that with this method hash codes could be repeated that
    probability is extremely low, and we didn't want to use external libraries*/
    @Override
    public int hashCode()
    {
        int i, hash_code = 1;

        for (i = 0; i < this.recording.length(); i++)
            hash_code += recording.charAt(i)*(i*i+1);

        return hash_code;
    }

    @Override
    public String toString()
    {
        return String.format("Position: %s\nRecording: %s\nArtist: %s\nTitle: %s\nDuration: %s", this.position, this.recording, this.artist, this.title, this.duration);
    }
}