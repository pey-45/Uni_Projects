package ej3;

public class Track {

    private String position, recording, artist, title;
    private int duration;

    //getters
    public String getPosition() { return this.position; }
    public String getRecording() { return this.recording; }
    public String getArtist() { return this.artist; }
    public String getTitle() { return this.title; }
    public int getDuration() { return this.duration; }

    //setters
    public void setPosition(String position)
    {
        if (position!=null) this.position = position;
        else throw new IllegalArgumentException();
    }
    public void setRecording(String recording)
    {
        if (recording!=null) this.recording = recording;
        else throw new IllegalArgumentException();
    }
    public void setArtist(String artist)
    {
        if (artist!=null) this.artist = artist;
        else throw new IllegalArgumentException();
    }
    public void setTitle(String title)
    {
        if (title!=null) this.title = title;
        else throw new IllegalArgumentException();
    }
    public void setDuration(int duration)
    {
        if (duration >= 0) this.duration = duration;
        else throw new IllegalArgumentException();
    }

    //constructor
    public Track(String position, String recording, String artist, String title, int duration)
    {
        if (position!=null && recording!=null && artist!=null && title!=null && duration>=0)
        {
            this.position = position;
            this.recording = recording;
            this.artist = artist;
            this.title = title;
            this.duration = duration;
        }
        else throw new IllegalArgumentException();
    }

    @Override
    public boolean equals(Object track)
    {
        //verifies if the classes are equal
        if (track.getClass() != Track.class)
            return false; //we doubt if this has to be an exception or not, we decided this is the best option

        //confirms that the variable we are using has class Release
        Track paramTrack = (Track) track;

        //verifies if the list sizes are equal
        if (this.recording.length() != paramTrack.recording.length())
            return false;

        //verifies for each character if they are equal
        for (int i = 0; i < this.recording.length(); i++)
        {
            if (this.recording.charAt(i) != paramTrack.recording.charAt(i))
                return false;
        }

        return true;
    }

    //knowing that with this method hash codes could be repeated that probability is extremely low, and we didn't want to use external libraries
    @Override
    public int hashCode()
    {
        int i, hash_code = 1;

        for (i = 0; i < this.recording.length(); i++)
            /*we multiply by i^2+1 because doing that we avoid repeating hash codes if for example two pair of characters are
            equal but are in different positions plus we make hash codes harder to repeat by creating more combinations*/
            hash_code += recording.charAt(i)*(i*i+1);

        return hash_code;
    }

    @Override
    public String toString()
    {
        return String.format("Position: %s\nRecording: %s\nArtist: %s\nTitle: %s\nDuration: %ss", this.position, this.recording, this.artist, this.title, this.duration);
    }
}