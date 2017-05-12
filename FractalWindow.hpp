#ifndef FRAKTALE_FRACTALWINDOW_HPP
#define FRAKTALE_FRACTALWINDOW_HPP

#include <QtWidgets/QDialog>
#include <QComboBox>
#include "Canvas.hpp"
#include "Fractals/Fractal.hpp"

namespace Fractals {
  enum ID : unsigned;
}

class QFormLayout;

/**
 * Dies ist das Hauptfenster, welches die Frak-
 * tale und zusätzliche Einstellungen anzeigt.
 */
class FractalWindow : public QDialog {
  // Setzt das Fenster als Q_OBJECT, so dass
  // Signal/Slot-Funktionalität gegeben ist.
  Q_OBJECT

  public slots:
    /**
     * Slot:
     * Zeichnet das Fraktal, welches zur Zeit
     * ausgewählt ist auf das Canvas.
     *
     * @slot
     * @see Canvas()
     */
    void draw();

    /**
     * Slot:
     * Erstellt ein neues Fraktal mit, welches der
     * ID fractalID zugeordnet wurde. Dabei wird
     * das Zentrum, der Zoom und Einstellungen
     * des vorherigen Fraktals zurückgesetzt.
     *
     * @param fractalID Id, welcher ein Fraktal mittels der Funktion
     *                  'registerFractal' zugeordnet wurde.
     *
     * @slot
     * @see registerFractal
     */
    void createFractal(Fractals::ID fractalID);

    /**
     * Mit dieser Funktion wird ein Datei-Browser geöffnet,
     * so dass ein Fraktal, welches zuvor exportiert
     * wurde, zu importieren.
     *
     * @see exportFractal
     */
    void importFractal();

    /**
     * Exportiert das aktuelle Fraktal in eine Datei.
     * Dabei werden auch aktuelle Einstellungen
     * des Fraktals gespeichert.
     *
     * @see importFractal
     */
    void exportFractal();
    /**
     * Slot:
     * Zeigt einen kleinen Indikator dafür, dass etwas gerade getan wird.
     */
    void showWaitingIndicator();
    /**
     * Slot:
     * Verbirgt den Indikator dafür, dass etwas gerade getan wird.
     */
    void hideWaitingIndicator();

  public:
    /**
     * Konstruktor, welcher eine Instanz von 'FractalWindow' erzeugt.
     *
     * @param parent Das QWidget, welches den Besitzt über
     *               die Instanz übernehmen sollte.
     */
    explicit FractalWindow(QWidget *parent = 0);

    /**
     * Registriert ein neues Fraktal mit der ID fractalID, welches
     * später mittels 'createFractal' erzeugt werden kann.
     *
     * @tparam T        Das Fraktal, welches instanziiert werden
     *                  soll, beim Ändern durch den Benutzer.
     * @param fractalID Die ID, welcher das zu erzeugende
     *                  Fratal zugewiesen werden soll.
     *
     * @see createFractal
     */
    template <typename T>
    void registerFractal(Fractals::ID fractalID, QString label);

  private:
    /**
     * Das Canvas, auf das Fraktale gezeichnet werden.
     */
    Canvas canvas;
    /**
     * Ein Pointer (unique_ptr), welcher auf das aktuelle Fraktal zeigt.
     */
    Fractal::Ptr currentFractal;
    /**
     * Variable, welche angibt, ob das Fraktal zur Zeit bewegt wird.
     */
    bool isDragging;
    /**
     * Die Letzte Position der Maus beim Bewegen des
     * Fraktals, in lokalen Koordinaten des Widgets.
     */
    QPoint lastDragPos;
    /**
     * Ein Pointer auf das Layout mit den Ein-
     * stellungen der einzelnen Fraktale.
     */
    QFormLayout* settings;
    /**
     * Ein Zeiger auf die Combobox zum Auswählen
     * verschiedener Fraktale.
     */
    QComboBox* fractalsCombo;
    /**
     * Eine Membervariable, welche Funktionen zum Erzeugen
     * von Fraktalen einer Frktal-ID zuordnet.
     */
    std::map<Fractals::ID, std::function<Fractal::Ptr()>> fractalFactory;

    /**
     * Erzeugt die Einstellungen des Widgets, wo man Fraktale
     * und die spezifischen Einstellungen wählen kann.
     * Dabei wird ein Pointer zurückgegeben, dessen
     * Eigentum selbst bestimmt werden muss.
     *
     * @return Ein Zeiger, welcher auf ein QWidget
     *         mit den Einstellungen deutet.
     */
    QWidget* createSettings();

    /**
     * Ein Event, welches ausgelöst wird, wenn
     * das Fenster in der Größe verändert wird.
     *
     * @param event
     */
    void resizeEvent(QResizeEvent *event) override;
    /**
     * Ein Event, welches ausgelöst wird,
     * wenn das Mausrad bewegt wird.
     *
     * @param event
     */
    void wheelEvent(QWheelEvent *event) override;
    /**
     * Überschreibt das Event beim Loslassen einer
     * Taste. Mit '+' und '-' kann so herein-
     * und herausgezoomt werden.
     *
     * @param event
     */
    void keyReleaseEvent(QKeyEvent *event) override;
    /**
     * Ein Event, welches ausgelöst wird,
     * wenn eine Maustaste gedrückt wird.
     *
     * @param event
     */
    void mousePressEvent(QMouseEvent *event) override;
    /**
     * Ein Event, welches ausgelöst wird, wenn
     * eine Maustaste losgelassen wird.
     *
     * @param event
     */
    void mouseReleaseEvent(QMouseEvent *event) override;
    /**
     * Ein Event, welches ausgelöst wird,
     * wenn die Maus bewegt wird.
     *
     * @param event
     */
    void mouseMoveEvent(QMouseEvent *event) override;
};

template<typename T>
void FractalWindow::registerFractal(Fractals::ID fractalID, QString label) {
  // Fügt einen Eintrag zu der Combobox der Fraktale hinzu.
  fractalsCombo->addItem(label, QVariant::fromValue(fractalID));
  // Speichert eine Funktion zum Erzeugen eines neuen Fraktals,
  // welche bei Bedarf durch AUsführen ein neues Fraktal generiert.
  fractalFactory[fractalID] = [this] () -> Fractal::Ptr {
    return Fractal::Ptr(new T(canvas.size().width(), canvas.size().height()));
  };
}

#endif //FRAKTALE_FRACTALWINDOW_HPP
