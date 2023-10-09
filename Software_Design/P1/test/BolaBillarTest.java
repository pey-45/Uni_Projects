package ej4;

import org.junit.jupiter.api.Test;

import static org.junit.jupiter.api.Assertions.*;

class BolaBillarTest {

    BolaBillar bola3 = BolaBillar.BOLA3;
    BolaBillar bola9 = BolaBillar.BOLA9;
    BolaBillar bola_blanca = BolaBillar.BLANCA;
    BolaBillar bola_negra = BolaBillar.BOLA8;
    @Test
    void testGetters()
    {
        assertEquals(bola3.getNum(), 3);
        assertEquals(bola9.getNum(), 9);
        assertEquals(bola_blanca.getNum(), 0);
        assertEquals(bola_negra.getNum(), 8);
    }

    @Test
    void testGetColor()
    {
        assertEquals(bola3.getColor(), "ROJO");
        assertEquals(bola9.getColor(), "AMARILLO");
        assertEquals(bola_blanca.getColor(), "BLANCO");
        assertEquals(bola_negra.getColor(), "NEGRO");
    }

    @Test
    void testGetType()
    {
        assertEquals(bola3.getType(), "LISA");
        assertEquals(bola9.getType(), "RAYADA");
        assertEquals(bola_blanca.getType(), "LISA");
        assertEquals(bola_negra.getType(), "LISA");
    }

    @Test
    void testValues()
    {
        assertEquals(BolaBillar.values()[2].toString(), "BOLA2");
        assertEquals(BolaBillar.values().length, 16);
    }

    @Test
    void testValueOf()
    {
        assertEquals(BolaBillar.valueOf("BOLA1").toString(), "BOLA1");
        assertEquals(BolaBillar.valueOf("BOLA8").toString(), "BOLA8");
    }
}