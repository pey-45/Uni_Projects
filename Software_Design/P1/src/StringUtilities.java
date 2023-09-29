public class StringUtilities
{
    public static boolean isValidString (String string, String valid_characters, int size)
    {
        boolean found;
        int i, j;

        //verifies if string is null, empty or smaller than the size
        if( string == null || string.isEmpty() || string.length() < size)
            return false;

        //verifies the rest of requirements
        for (i = 0; i < string.length(); ++i)
        {
            //is it a digit?
            if (string.charAt(i)>=48 && string.charAt(i)<=57)
                continue;

            //is it in the valid characters string?
            found = false;
            for (j = 0; j < valid_characters.length(); ++j)
            {
                if (string.charAt(i)==valid_characters.charAt(j))
                    found = true;
            }
            if (!found)
                return false;
        }

        return true;
    }

    public static String lowercaseFirst(String string)
    {
        int i;
        StringBuilder newString = new StringBuilder();
        StringBuilder uppercaseString = new StringBuilder();

        //concatenates lowercase and uppercase characters to them corresponding string
        for (i = 0; i<string.length(); ++i)
        {
            if(Character.isLowerCase(string.charAt(i)))
                newString.append(string.charAt(i));
            else
                uppercaseString.append(string.charAt(i));
        }

        //concatenates the lowercase after the uppercase
        newString.append(uppercaseString);
        return newString.toString();
    }

    public static boolean checkTextStats (String string, int min, int max)
    {
        //if the string is not valid throws illegal exception
        if (string == null || string.isEmpty() || min<=0 || max<=0)
            throw new IllegalArgumentException();

        //stores the words of the string into an array of strings
        String[] split_string = string.split("\\s+");
        String max_string = "";
        int counter = 0, i;

        //stores the total number of characters and the bigger string
        for (i = 0; i<split_string.length; ++i)
        {
            counter += split_string[i].length();
            if (split_string[i].length()>max_string.length())
                max_string = split_string[i];
        }

        //calculates the average string size and returns true if the conditions were satisfied and false if they were not
        double avg = (double)counter/(double)split_string.length;
        return !(avg < min) && !(avg > max) && !(2 * avg < max_string.length());
    }
}