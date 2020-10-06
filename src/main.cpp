#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "main.h"
#include "city.h"
#include "datapoint.h"

#define screenWidth 1920
#define screenHeight 1080
#define bgColor al_map_rgb(0, 0, 0)
#define whiteBgColor al_map_rgb(255, 255, 255)
#define refreshRate 60

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;

ALLEGRO_BITMAP *bgImage;
ALLEGRO_SAMPLE *bgMusic;
ALLEGRO_SAMPLE_INSTANCE *bgMusicInstance;

int mouseX = 0;
int mouseY = 0;

using namespace std;

int startUp() {

    if (!al_init()) {
        al_show_native_message_box(nullptr , "Pinpoint++", nullptr,
                                   "Could not initialize Allegro", nullptr, 0);
        return 1;
    }

    al_install_mouse();
    al_install_audio();

    al_init_primitives_addon();
    al_init_image_addon();
    al_init_acodec_addon();

    display = al_create_display(screenWidth, screenHeight);
    timer = al_create_timer(1.0f/refreshRate);
    event_queue = al_create_event_queue();

    if (!timer or !display or !event_queue) {
        al_show_native_message_box(nullptr, "Pinpoint++", nullptr,
                                   "Error Initialing Timer, Display or Event Queue", nullptr, 0);
        return 1;
    }

    bgImage = al_load_bitmap("/Users/iobruno/Vault/senac/allegro-pinpoint/assets/images/world_map.jpg");
    if (!bgImage) {
        al_show_native_message_box(nullptr, "Pinpoint++", nullptr,
                                   "Error Loading Background Image", nullptr, 0);
        return 1;
    }

    bgMusic = al_load_sample("/Users/iobruno/Vault/senac/allegro-pinpoint/assets/audio/tracks/POL-misty-dungeon-short.ogg");
    bgMusicInstance = al_create_sample_instance(bgMusic);
    al_reserve_samples(2);
    al_set_sample_instance_playmode(bgMusicInstance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(bgMusicInstance, al_get_default_mixer());

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_play_sample_instance(bgMusicInstance);

    al_start_timer(timer);
    redrawScreen();
    return 0;
}

void redrawScreen() {
    al_draw_scaled_bitmap(bgImage,
                          0, 0, 1375, 972,
                          0, 0, screenWidth, screenHeight, 0);
    al_draw_circle(float(mouseX), float(mouseY), 10, whiteBgColor, 5);

    al_flip_display();
    al_clear_to_color(bgColor);
}

void destroy() {
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(bgImage);
    al_destroy_sample(bgMusic);
    al_destroy_sample_instance(bgMusicInstance);
}


int main(int argc, char** argv) {
    bool isRunning = true;
    bool needsRedrawing = true;

    Datapoint datapoint = Datapoint::loadDataPointsFrom("");
    City *city = datapoint.pickRandomCity();
    fprintf(stdout, "Selected City: %s (%d, %d)\n", city->getName().c_str(), city->getPosX(), city->getPosY());

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
                float dist = city->computeDistanceFrom(mouseX, mouseY);
                fprintf(stdout, "Click on Coordinates: (%d, %d)\n", mouseX, mouseY);
                fprintf(stdout, "Distance to %s (%d, %d): %f\n",
                        city->getName().c_str(), city->getPosX(), city->getPosY(), dist);

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
    if (al_is_event_queue_empty(event_queue)) {
        redrawScreen();
        fprintf(stdout, "Event Queue is empty - Redrawing");
    } else if (needsRedrawing) {
        redrawScreen();
        needsRedrawing = false;
    }

    destroy();
    return 0;
}
