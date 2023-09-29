// Press Shift twice to open the Search Everywhere dialog and type `show whitespaces`,
// then press Enter. You can now see whitespace characters in your code.
public class Main
{
    public static void main(String[] args)
    {
        ImmutableMatrix im = new ImmutableMatrix(3,4);
        im = im.transpose();

        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 3; ++j)
                System.out.println(im.toArray2D()[i][j]);
        }
    }
}