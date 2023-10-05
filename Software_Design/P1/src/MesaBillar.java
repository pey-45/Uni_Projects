package ej4;

import java.util.ArrayList;

public class MesaBillar
{
    private ArrayList<BolaBillar> bolas;
    int i;

    public MesaBillar()
    {
        for (i = 0; i < 16; i++)
        {
            bolas.add(new BolaBillar(i));
        }
    }
}
