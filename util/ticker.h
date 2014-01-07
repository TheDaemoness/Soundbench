#ifndef TICKER_H
#define TICKER_H

namespace sb {
    template <typename T = float>
    class Ticker<T> {
    private:
        double pos;
        float speed;
    public:
        Ticker() {
            pos = 0;
            speed = 0;
        }
        void setPos(T p) {
            pos = static_cast<double>(p);
        }
        void setSpeed(float s) {
            speed = s;
        }
        T tick() {
            pos += speed;
            return static_cast<T>(pos);
        }
        T getValue() {
            return static_cast<T>(pos);
        }
    };
}

#endif // TICKER_H
