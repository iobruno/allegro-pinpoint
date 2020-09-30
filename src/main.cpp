#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#define screenWidth 1920
#define screenHeight 1080
#define bgColor al_map_rgb(0, 0, 0)
#define whiteBgColor al_map_rgb(255, 255, 255)
#define refreshRate 1.0f/60

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;

int mouseX = 0;
int mouseY = 0;


int startUp();
void redrawScreen();
void destroy();

int main(int argc, char** argv) {
    bool isRunning = true;
    bool needsRedrawing = true;

    startUp();

    while (isRunning) {
        ALLEGRO_EVENT events;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.06);
        al_wait_for_event_until(event_queue, &events, &timeout);

        switch(events.type) {
            case ALLEGRO_EVENT_MOUSE_AXES: {
                mouseX = events.mouse.x;
                mouseY = events.mouse.y;
                break;
            }
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                fprintf(stdout, "Click on Coordinates: (%d, %d)\n", mouseX, mouseY);
                al_draw_circle(float(mouseX), float(mouseY), 10, whiteBgColor, 1);
                redrawScreen();
                break;
            }
            case ALLEGRO_EVENT_TIMER: {
                needsRedrawing = true;
                break;
            }
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                isRunning = false;
            }
        }
    }

    // Check if we need to needsRedrawing
    if (needsRedrawing && al_is_event_queue_empty(event_queue)) {
        redrawScreen();
        needsRedrawing = false;
    }

    destroy();
    return 0;
}

/**
 * Intializestimer, display and eventQueue
 * @return int
 */
int startUp() {

    if (!al_init()) {
        al_show_native_message_box(nullptr , "Pinpoint++", nullptr,
                                   "Could not initialize Allegro", nullptr, 0);
        return 1;
    }

    display = al_create_display(screenWidth, screenHeight);
    timer = al_create_timer(refreshRate);
    event_queue = al_create_event_queue();

    if (!timer or !display or !event_queue) {
        al_show_native_message_box(nullptr, "Pinpoint++", nullptr,
                                   "Error Initialing Timer, Display or Event Queue", nullptr, 0);
        return 1;
    }
    else {
        al_init_primitives_addon();
        al_install_mouse();
        al_register_event_source(event_queue, al_get_timer_event_source(timer));
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_mouse_event_source());
        al_start_timer(timer);

        redrawScreen();
    }

    return 0;
}

/**
 * Draws/Redraws screen bgColor
 */
void redrawScreen() {
    al_flip_display();
    al_clear_to_color(bgColor);
}

/**
 * Finishes the Application
 */
void destroy() {
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}
