package ej3;

import java.util.ArrayList;

public class Release
{

    private String releaseID, artist, title;
    private ArrayList<Track> track_list;

    //getters
    public String getReleaseID() { return releaseID; }
    public String getArtist() { return artist; }
    public String getTitle() { return title; }
    public ArrayList<Track> getTrack_list() { return track_list; }

    //setters
    public void setReleaseID(String releaseID)
    {
        if (releaseID!=null) this.releaseID = releaseID;
        else throw new IllegalArgumentException();
    }
    public void setTrack_list(ArrayList<Track> track_list) { this.track_list = track_list; }
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

    //constructor
    public Release(String releaseID)
    {
        if (releaseID!=null)
        {
            this.releaseID = releaseID;
            this.track_list = new ArrayList<>();
        }
        else throw new IllegalArgumentException();
    }

    public void addTrack(Track track) { this.track_list.add(track); }

    @Override
    public boolean equals(Object release)
    {
        boolean found;
        int i, j;
        //track_list_size stands for the affected track list size, not the parameter
        final int track_list_size = this.track_list.size();

        //verifies if the classes are equal
        if(release.getClass() != Release.class)
            return false; //we doubt if this has to be an exception or not, we decided this is the best option

        //confirms that the variable we are using has class Release
        Release paramRelease = (Release) release;

        //verifies if the list sizes are equal
        if(track_list_size != paramRelease.track_list.size())
            return false;

        /*verifies that every track in the first track list is in the other one,
        knowing that they can't repeat ant the list size is the same*/
        for (i = 0; i < track_list_size; i++)
        {
            found = false;

            for (j = 0; j < track_list_size; j++)
            {
                //uses the Track equals function, not the default one
                if (paramRelease.track_list.get(i).equals(this.track_list.get(j)))
                {
                    found = true;
                    break;
                }
            }

            if(!found)
                return false;
        }

        return true;
    }

    @Override
    public int hashCode()
    {
        int i, hash_code = 1;

        //runs the track list and sums the hash_codes to create a new one
        //same as the another one, this method is not perfect, but the probability of getting two equal hash codes is extremely low
        for (i = 0; i < this.track_list.size(); i++)
            hash_code += this.track_list.get(i).hashCode();

        return hash_code;
    }

    @Override
    public String toString()
    {
        int i;
        String string = String.format("%s: %s from %s\n\n", this.releaseID, this.title, this.artist);
        StringBuilder aux = new StringBuilder(string);

        for (i = 0; i < this.track_list.size(); i++)
            aux.append(String.format("Track %d:\n\n%s\n\n", i+1, this.track_list.get(i).toString()));

        return aux.toString();
    }
}