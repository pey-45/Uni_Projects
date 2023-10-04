package ej4;

public class BolaBillar
{
    private enum Bola
    {
        BLANCA (0, "BLANCO", "LISA"),
        BOLA1 (1, "AMARILLO", "LISA"),
        BOLA2 (2, "AZUL", "LISA"),
        BOLA3 (3, "ROJO", "LISA"),
        BOLA4 (4, "VIOLETA", "LISA"),
        BOLA5 (5, "NARANJA", "LISA"),
        BOLA6 (6, "VERDE", "LISA"),
        BOLA7 (7, "GRANATE", "LISA"),
        BOLA8 (8, "NEGRO", "LISA"),
        BOLA9 (9, "AMARILLO", "RAYADA"),
        BOLA10 (10, "AZUL", "RAYADA"),
        BOLA11 (11, "ROJO", "RAYADA"),
        BOLA12 (12, "VIOLETA", "RAYADA"),
        BOLA13 (13, "NARANJA", "RAYADA"),
        BOLA14 (14, "VERDE", "RAYADA"),
        BOLA15 (15, "GRANATE", "RAYADA");

        private final int num;
        private final String color;
        private final String type;

        Bola (int num, String color, String type)
        {
            if (num >= 0 && num <= 15 && color!=null && type!=null)
            {
                this.num = num;
                this.color = color;
                this.type = type;
            }
            else throw new IllegalArgumentException();
        }

        public int getNum() { return this.num; }
        public String getColor() { return this.color; }
        public String getType() { return this.type; }
    }

    public BolaBillar(Bola bola)
    {

    }
}
