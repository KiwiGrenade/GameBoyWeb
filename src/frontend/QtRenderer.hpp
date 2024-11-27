#include <QObject>
#include <QImage>
#include <QPixmap>
#include <mutex>

#include "GraphicTypes.hpp"
#include "Renderer.hpp"
#include "utils.hpp"

class QtRenderer : public QObject, public Renderer {
    Q_OBJECT

public:
    explicit QtRenderer(u16 w, u16 h, QObject *parent = nullptr);

    void drawTexture(const Texture &tex, u16 x = 0, u16 y = 0) override;
    void clear();
    QImage getImage() const;
    QPixmap getPixmap() const;

signals:
    void showScreen() override;

private:
    mutable std::mutex bufMutex_;
    u16 width_;
    u16 height_;
    std::vector <u8> buf_;
};
