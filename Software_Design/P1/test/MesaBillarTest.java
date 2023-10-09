package ej4;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class MesaBillarTest
{
    MesaBillar mesa = new MesaBillar();

    @Test
    void testIniciarPartida()
    {
        assertEquals(mesa.bolasCajetin().size(), 16);
        assertEquals(mesa.bolasMesa().size(), 0);
        mesa.iniciarPartida();
        assertEquals(mesa.bolasCajetin().size(), 0);
        assertEquals(mesa.bolasMesa().size(), 16);
    }

    @Test
    void testMeterBola()
    {
        mesa.iniciarPartida();
        mesa.meterBola(mesa.bolasMesa().get(2));
        assertEquals(mesa.bolasCajetin().size(), 1);
        assertEquals(mesa.bolasCajetin().get(0).toString(), "BOLA2");
        mesa.meterBola(mesa.bolasMesa().get(0));
        assertEquals(mesa.bolasCajetin().size(), 1);
        assertTrue(mesa.isPartidaIniciada());
        mesa.meterBola(mesa.bolasMesa().get(7));
        assertEquals(mesa.bolasCajetin().size(), 1);
        assertFalse(mesa.isPartidaIniciada());
    }

    @Test
    void testBolasMesaYCajetin()
    {
        assertEquals(mesa.bolasMesa().size(), 0);
        assertEquals(mesa.bolasCajetin().size(), 16);
        mesa.iniciarPartida();
        assertEquals(mesa.bolasMesa().size(), 16);
        assertEquals(mesa.bolasCajetin().size(), 0);
        mesa.meterBola(mesa.bolasMesa().get(2));
        assertEquals(mesa.bolasMesa().size(), 15);
        assertEquals(mesa.bolasCajetin().size(), 1);
    }

    @Test
    void isPartidaIniciada()
    {
        assertFalse(mesa.isPartidaIniciada());
        mesa.iniciarPartida();
        assertTrue(mesa.isPartidaIniciada());
    }

    @Test
    void obtenerGanador()
    {
        assertEquals(mesa.obtenerGanador(), "EMPATE");
        mesa.iniciarPartida();
        assertEquals(mesa.obtenerGanador(), "EMPATE");
        mesa.meterBola(mesa.bolasMesa().get(0));
        assertEquals(mesa.obtenerGanador(), "EMPATE");
        mesa.meterBola(mesa.bolasMesa().get(1));
        assertEquals(mesa.obtenerGanador(), "LISAS");
        mesa.meterBola(mesa.bolasMesa().get(9));
        mesa.meterBola(mesa.bolasMesa().get(9));
        assertEquals(mesa.obtenerGanador(), "RAYADAS");
        assertTrue(mesa.isPartidaIniciada());
        mesa.meterBola(mesa.bolasMesa().get(7));
        assertFalse(mesa.isPartidaIniciada());
        assertEquals(mesa.obtenerGanador(), "RAYADAS");
        mesa.iniciarPartida();
        assertTrue(mesa.isPartidaIniciada());
        assertEquals(mesa.obtenerGanador(), "EMPATE");
    }
}