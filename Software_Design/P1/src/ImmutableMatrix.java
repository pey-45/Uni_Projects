public class ImmutableMatrix
{
    private final int[][] arr;

    public ImmutableMatrix(int[][] arr)
    {
        int i;

        //verifies if the matrix is not ragged
        for (i = 0; i<arr.length; ++i)
        {
            if (arr[i].length!=arr[0].length)
                throw new IllegalArgumentException();
        }
        this.arr = arr;
    }

    public ImmutableMatrix(int rows, int cols)
    {
        //throws illegal exception if any dimension is under 1
        if (rows < 1 || cols < 1)
            throw new IllegalArgumentException();

        this.arr = new int[rows][cols];

        //runs the matrix and assigns ascendant values from 1
        int i, j, cnt = 1;
        for (i = 0; i<rows; ++i)
        {
            for (j = 0; j<cols; ++j)
                this.arr[i][j] = cnt++;
        }
    }

    public String toString()
    {
        StringBuilder string = new StringBuilder();
        int i, j;

        //runs the matrix and converts it to string format
        for (i = 0; i<this.arr.length; ++i)
        {
            string.append("[");
            for (j = 0; j<this.arr[i].length; ++j)
                string.append(String.format(j < this.arr[i].length - 1 ? "%d, " : "%d]", this.arr[i][j]));
            string.append("\n");
        }

        return string.toString();
    }

    public int at(int i, int j)
    {
        //if any coordinate is out of bounds throws illegal exception
        if (i > this.arr.length || j > this.arr[0].length || i < 0 || j < 0)
            throw new IllegalArgumentException();

        return this.arr[i][j];
    }

    public int rowCount()
    {
        return this.arr.length;
    }

    public int columnCount()
    {
        return this.arr[0].length;
    }

    public int[][] toArray2D()
    {
        return this.arr;
    }

    public ImmutableMatrix reverse()
    {
        int i, j;
        int[][] reversed_arr = new int[this.arr.length][this.arr[0].length];

        //runs the matrix and assigns the even symmetric to each position
        for (i = 0; i<this.arr.length; ++i)
        {
            for (j = 0; j<this.arr[i].length; ++j)
                reversed_arr[i][j] = this.arr[i][this.arr[i].length-1-j];
        }

        return new ImmutableMatrix(reversed_arr);
    }

    public ImmutableMatrix transpose()
    {
        int i, j;
        int[][] reversed_arr = new int[this.arr[0].length][this.arr.length];

        //runs the matrix and assigns the odd symmetric to each position
        for (i = 0; i<this.arr.length; ++i)
        {
            for (j = 0; j<this.arr[i].length; ++j)
                reversed_arr[j][i] = this.arr[i][j];
        }

        return new ImmutableMatrix(reversed_arr);
    }

    public ImmutableMatrix reshape(int newColumns)
    {
        int rows = this.arr.length, cols = this.arr[0].length, i, j, count = 0;
        int[] array_1d = new int[rows*cols];
        int[][] reshaped_arr = new int[rows*cols/newColumns][newColumns];

        //verifies if it can be reshaped
        if (rows * cols % newColumns!=0)
            throw new IllegalArgumentException();

        //creates a uni-dimensional array with all the values ordered
        for (i = 0; i < rows; ++i)
        {
            for (j = 0; j < cols; ++j)
                array_1d[count++] = this.arr[i][j];
        }

        //assigns these values to the reshaped array
        count = 0;
        for (i = 0; i < rows * cols / newColumns; ++i)
        {
            for (j = 0; j<newColumns; ++j)
                reshaped_arr[i][j] = array_1d[count++];
        }

        return new ImmutableMatrix(reshaped_arr);
    }
}
