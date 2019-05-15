#ifndef ANCHORWIDGET_HPP
#define ANCHORWIDGET_HPP

#include <QWidget>
#include <QMap>

QT_FORWARD_DECLARE_CLASS(QButtonGroup);

namespace Ui {
class AnchorWidget;
}

enum Anchor {
    TopLeft,
    Top,
    TopRight,
    Left,
    Center,
    Right,
    BottomLeft,
    Bottom,
    BottomRight
};

class AnchorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AnchorWidget(QWidget *parent = nullptr);
    ~AnchorWidget();

    Anchor anchor() const;
    void setAnchor(Anchor anchor);

private slots:
    void selectTopLeft();
    void selectTop();
    void selectTopRight();
    void selectLeft();
    void selectCenter();
    void selectRight();
    void selectBottomLeft();
    void selectBottom();
    void selectBottomRight();

private:
    Ui::AnchorWidget *ui;

    QButtonGroup *buttonGroup;
    QMap<int, Anchor> map;
};

#endif // ANCHORWIDGET_HPP
