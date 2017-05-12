#ifndef FRAKTALE_CANVAS_HPP
#define FRAKTALE_CANVAS_HPP

#include <QtWidgets/QWidget>

/**
 * Eine Zeichenfläfe, auf die Objekte vom
 * Typ QImage gezeichnet werden können.
 */
class Canvas : public QWidget {
  public:
    /**
     * Zeichnet ein QImage auf die Zeichenfläche.
     *
     * @param image Das Bild, welches gezeichnet werden soll.
     */
    void draw(QImage& image);

    //void drawOverlay(QString info);
    void drawOverlay(QStringList infoLines);

  protected:
    /**
     * Das Event, welches für das eigentliche Zeichnen
     * verantwortlich ist. Kann mittels 'update()'
     * ausgelöst werden.
     *
     * @param event
     */
    void paintEvent(QPaintEvent *event) override;

  private:
    /**
     * Pixmap, welche von einem QImage erstellt und
     * auf den Bildschirm angezeigt wird.
     */
    QPixmap pixmap;
};


#endif //FRAKTALE_CANVAS_HPP
