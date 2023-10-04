package ej3;

import java.util.ArrayList;

public class Release
{

    private String releaseID, artist, title;
    private ArrayList<Track> track_list;

    public String getReleaseID() {
        return releaseID;
    }

    public String getArtist() {
        return artist;
    }

    public String getTitle() {
        return title;
    }

    public ArrayList<Track> getTrack_list() {
        return track_list;
    }

    public void setReleaseID(String releaseID) {
        this.releaseID = releaseID;
    }

    public void setTrack_list(ArrayList<Track> track_list) {
        this.track_list = track_list;
    }

    public void setArtist(String artist){
        this.artist = artist;
    }

    public void setTitle(String title){
        this.title = title;
    }


    public Release(String releaseID)
    {
        this.releaseID = releaseID;
        track_list = new ArrayList<>();
    }

    public void addTrack(Track track){
        this.track_list.add(track);
    }

    @Override
    public boolean equals(Object release)
    {
        boolean found;
        int i, j, track_list_size = this.track_list.size();
        /*track_list_size stands for the
        list of the affected track list size, not the parameter*/

        //verifies if the classes are equal
        if(release.getClass() != Release.class)
            return false;

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
                if(paramRelease.track_list.get(i).equals(this.track_list.get(j)))
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
        /*same as the another one, this method is not perfect, but the
        probability of getting two equal hash codes is extremely low*/
        for (i = 0; i < this.track_list.size(); i++)
            hash_code += this.track_list.get(i).hashCode();

        return hash_code;
    }

    @Override
    public String toString()
    {
        int i;
        StringBuilder string = new StringBuilder();
        string.append(this.releaseID).append(": ").append(this.title).append(" from ").append(this.artist).append("\n\n");
        for (i = 0; i < this.track_list.size(); i++)
            string.append("Track ").append((char)(i+1)).append("\n\n").append(this.track_list.get(i).toString());

        return string.toString();
    }
}
