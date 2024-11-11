#include <QObject>
#include <QImage>
#include <QPixmap>
#include <mutex>

#include "GraphicTypes.hpp"
#include "utils.hpp"

class QtRenderer : public QObject {
    Q_OBJECT
    
    public:
    explicit QtRenderer(u16 w, u16 h, QObject *parent = nullptr);

    void drawTexture(const Texture& tex, u16 x = 0, u16 y = 0);

    void clear();

    QImage getImage() const;
    QPixmap getPixmap() const;

    signals:
        void showScreen();

private:
    mutable std::mutex buf_mutex_;
    u16 width_;
    u16 height_;
    std::vector<u8> buf_;
};
