#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <QMetaType>

namespace Channel {
    enum Color {
        red = 0x1,
        green = 0x2,
        blue = 0x4,
        cyan = 0x10,
        yellow = 0x20,
        magenta = 0x40,
        black = 0x80,
        rgb = red | green | blue,
        cmyk = cyan | magenta | yellow | black,
    };
}

Q_DECLARE_METATYPE(Channel::Color)

#endif // CHANNEL_HPP
