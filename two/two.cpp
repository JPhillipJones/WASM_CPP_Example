class Asteroid
{
        public:
            Asteroid() {}
            Asteroid(int x, int y, int radius)
            {
                X = x; Y = y; Radius = radius;
            }
            int X;
            int Y;
            int Radius;
};