#ifndef FRAKTALE_EMPTYFRACTAL_HPP
#define FRAKTALE_EMPTYFRACTAL_HPP

#include "Fractal.hpp"

/**
 * Ein leeres "Fraktal", welches nur Informationens-Text anzeigt.
 */
class EmptyFractal : public Fractal {
  public:
    /**
     * Erzeugt eine neue Instanz von einem leeren Fraktal.
     *
     * @param width Die Breite der Zeichenfläche.
     * @param height Die Breite der Zeichenfläche.
     */
    EmptyFractal(int width = 1, int height = 1);
    void update() override;
    /**
     * Zeichnet das Fraktal auf eine Zeichenfläche.
     *
     * Dabei wird nur ein Text gezeichnet, mit dem Hinweis, dass Fraktale
     * rechts in den Einstellungen ausgewält werden können.
     *
     * @param target Zeichenfläche, auf die das Fraktal gezeichnet werden soll.
     */
    void draw(Canvas& target) override;

    /**
     * Liefert die Einstellungen des Fraktals, welche verändert werden können.
     *
     * Das Fraktal hat keine Einstellungen, welche man anpassen kann.
     *
     * @return Eine Gruppe von QWidgets, welche diverse Einstellungen am Fraktal verändern können.
     */
    std::map<QString, QWidget *> getSettings() override;
};

#endif //FRAKTALE_EMPTYFRACTAL_HPP
