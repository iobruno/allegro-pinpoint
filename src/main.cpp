#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>

#define screenWidth 1920
#define screenHeight 1080
#define bgColor al_map_rgb(0, 0, 0)
#define refreshRate 1.0f/60

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;

int startUp();
void redrawScreen();
void destroy();

int main(int argc, char** argv) {
    bool isRunning = true;
    bool needsRedrawing = true;

    startUp();
    redrawScreen();
    al_start_timer(timer);

    while (isRunning) {
        ALLEGRO_EVENT event;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.06);

        if (al_wait_for_event_until(event_queue, &event, &timeout)) {
            switch (event.type) {
                case ALLEGRO_EVENT_TIMER:
                    needsRedrawing = true;
                    break;
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    isRunning = false;
                    break;
                default:
                    fprintf(stderr, "Unsupported event received: %d\n", event.type);
                    break;
            }
        }

        // Check if we need to needsRedrawing
        if (needsRedrawing && al_is_event_queue_empty(event_queue)) {
            redrawScreen();
            needsRedrawing = false;
        }
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
                                   "Could not initialize Allegro", nullptr, NULL);
        return 1;
    }

    display = al_create_display(screenWidth, screenHeight);
    timer = al_create_timer(refreshRate);
    event_queue = al_create_event_queue();

    if (!timer or !display or !event_queue) {
        al_show_native_message_box(nullptr, "Pinpoint++", nullptr,
                                   "Error Initialing Timer, Display or Event Queue", nullptr, NULL);
        return 1;
    }
    else {
        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_timer_event_source(timer));
    }

    return 0;
}

/**
 * Draws/Redraws screen bgColor
 */
void redrawScreen() {
    al_clear_to_color(bgColor);
    al_flip_display();
}

/**
 * Finishes the Application
 */
void destroy() {
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
}
