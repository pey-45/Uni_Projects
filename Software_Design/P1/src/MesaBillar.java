package ej4;

import java.util.ArrayList;
import java.util.Arrays;

public class MesaBillar
{
    private final ArrayList<BolaBillar> ballsOverTable = new ArrayList<>();
    private final ArrayList<BolaBillar> ballsInBox = new ArrayList<>();
    private boolean partidaIniciada = false;


    public MesaBillar()
    {
        ballsInBox.addAll(Arrays.asList(BolaBillar.values()));
    }

    public void iniciarPartida()
    {
        ballsInBox.clear();
        ballsOverTable.addAll(Arrays.asList(BolaBillar.values()));
        partidaIniciada = true;
    }

    public void meterBola(BolaBillar bola)
    {
        if (!partidaIniciada)
            throw new IllegalArgumentException();

        if (bola.getColor().equals("BLANCO"))
            return;

        if (bola.getColor().equals("NEGRO"))
            partidaIniciada = false;
        else
        {
            ballsOverTable.remove(bola);
            ballsInBox.add(bola);
        }
    }

    public ArrayList<BolaBillar> bolasMesa() { return ballsOverTable; }

    public ArrayList<BolaBillar> bolasCajetin() { return ballsInBox; }

    public boolean isPartidaIniciada() { return partidaIniciada; }

    public String obtenerGanador()
    {
        int i, n_rayadas = isPartidaIniciada()? 0:2, n_lisas = 0;

        for (i = 0; i < ballsInBox.size(); i++)
        {
            if (ballsInBox.get(i).getColor().equals("WHITE") || ballsInBox.get(i).getColor().equals("BLACK"))
                continue;

            if (ballsInBox.get(i).getType().equals("RAYADA"))
                n_rayadas++;
            else
                n_lisas++;
        }

        if (n_rayadas==n_lisas)
            return "EMPATE";

        return n_rayadas > n_lisas? "RAYADAS":"LISAS";
    }
}