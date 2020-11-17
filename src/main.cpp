#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>
#include "main.h"
#include "city.h"
#include "datapoint.h"

#define screenWidth 1920
#define screenHeight 1080
#define blackBgColor al_map_rgb(0, 0, 0)
#define whiteBgColor al_map_rgb(255, 255, 255)
#define refreshRate 30

ALLEGRO_DISPLAY *display;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *timer;

ALLEGRO_BITMAP *bgImage;
ALLEGRO_SAMPLE *bgMusic;
ALLEGRO_SAMPLE_INSTANCE *bgMusicInstance;
ALLEGRO_FONT *font;

int mouseX = 0, mouseY = 0;
double timeLeft, startTime;
bool isGameOver = false;

int score = 0;
int lifeAttempts = 5;
int timePerAttempt = 5; // in Seconds
int timerBarReduction = 0;

City *city = nullptr;


using namespace std;

int main(int argc, char **argv) {

    Datapoint datapoint = Datapoint::loadDataPointsFrom(
            "/Users/iobruno/Vault/github/allegro-pinpoint/assets/datasets/cities.csv");
    bool isRunning = true;

    initializeModules();

    while (isRunning) {
        ALLEGRO_EVENT events;
        ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.06);
        al_wait_for_event_until(event_queue, &events, &timeout);

        switch (events.type) {
            case ALLEGRO_EVENT_MOUSE_AXES: {
                mouseX = events.mouse.x;
                mouseY = events.mouse.y;
                break;
            }
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
                if (isGameOver) {
                    isRunning = false;
                } else if (city != nullptr) {
                    double dist = city->computeDistanceFrom(mouseX, mouseY);
                    score += computeScore(dist);
                    fprintf(stdout, "Click on Coordinates: (%d, %d)\n", mouseX, mouseY);
                    fprintf(stdout, "Distance to %s (%d, %d): %f\n\n",
                            city->getName().c_str(), city->getPosX(), city->getPosY(), dist);
                    city = nullptr;
                    timerBarReduction = 0;
                }
                break;
            }
            case ALLEGRO_EVENT_TIMER: {
                if (!isGameOver)
                    timeLeft = computeSecsLeft();

                /** All attempts are gone, Game Over  */
                if (lifeAttempts <= 0) {
                    gameOver();
                    break;
                }

                /**
                 * You still got some life left on you, so... if there's more datapoints available,
                 * continue, otherwise you bet the game
                 */
                if (city == nullptr) {
                    if (datapoint.isEmpty()) {
                        gameWon(); // Game Won
                    } else {
                        city = datapoint.pickRandomCity(); // Continue the Game
                        startTime = al_get_time();
                    }
                    break;
                }

                /** Default clause, the game is running and the clock is ticking */
                if (timeLeft <= 0.0) {
                    lifeAttempts -= 1;
                    city = datapoint.pickRandomCity();
                    startTime = al_get_time();
                }

                redrawScreen();
                break;
            }
            case ALLEGRO_EVENT_DISPLAY_CLOSE: {
                isRunning = false;
            }
        }
    }

    if (al_is_event_queue_empty(event_queue)) {
        redrawScreen();
        fprintf(stdout, "Event Queue is empty - Redrawing");
    }

    destroy();
    return 0;
}

void initializeModules() {
    if (!al_init()) {
        al_show_native_message_box(nullptr, "Pinpoint++", nullptr,
                                   "Could not initialize Allegro", nullptr, 0);
        exit(1);
    }

    al_install_mouse();
    al_install_audio();

    al_init_primitives_addon();
    al_init_image_addon();
    al_init_acodec_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    display = al_create_display(screenWidth, screenHeight);
    timer = al_create_timer(1.0f / refreshRate);
    event_queue = al_create_event_queue();

    if (!timer or !display or !event_queue) {
        al_show_native_message_box(nullptr, "Pinpoint++", nullptr,
                                   "Error Initialing Timer, Display or Event Queue", nullptr, 0);
        exit(1);
    }

    /** Loads Background Image */
    bgImage = al_load_bitmap("/Users/iobruno/Vault/github/allegro-pinpoint/assets/images/world_map.jpg");
    if (!bgImage) {
        al_show_native_message_box(nullptr, "Pinpoint++", nullptr,
                                   "Error Loading Background Image", nullptr, 0);
        exit(1);
    }

    /** Loads Background Music */
    bgMusic = al_load_sample(
            "/Users/iobruno/Vault/github/allegro-pinpoint/assets/audio/tracks/POL-misty-dungeon-short.ogg");
    bgMusicInstance = al_create_sample_instance(bgMusic);
    al_reserve_samples(2);
    al_set_sample_instance_playmode(bgMusicInstance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(bgMusicInstance, al_get_default_mixer());
    if (!bgMusic || !bgMusicInstance) {
        al_show_native_message_box(nullptr, "Pinpoint++", nullptr,
                                   "Error Loading Audio Sample", nullptr, 0);
        exit(1);
    }

    /** Loads TrueTypeFont */
    font = al_load_font("/Users/iobruno/Vault/github/allegro-pinpoint/assets/fonts/Arcade_Interlaced.ttf", 32, 0);
    if (!font) {
        al_show_native_message_box(nullptr, "Pinpoint++", nullptr,
                                   "Error Loading TTF Font", nullptr, 0);
        exit(1);
    }

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_play_sample_instance(bgMusicInstance);
    al_start_timer(timer);
    redrawScreen();
}

void redrawScreen() {
    if (!isGameOver) {
        al_draw_scaled_bitmap(bgImage,
                              0, 0, 1375, 972,
                              0, 0, screenWidth, screenHeight - 50, 0);

        drawHUD();
        drawTimeBar();
        al_draw_circle(float(mouseX), float(mouseY), 10, whiteBgColor, 5);
        al_flip_display();
        al_clear_to_color(blackBgColor);
    }
}

void drawTimeBar() {
    if (timeLeft <= 0.0) {
        timerBarReduction = 0;
    }
    timerBarReduction += screenWidth / (refreshRate * timePerAttempt);
    float timerBarWidth = screenWidth - timerBarReduction;
    al_draw_line(0, 0, timerBarWidth, 0, al_color_name("red"), 20);
}

void drawHUD() {
    al_draw_text(font, al_map_rgb(255, 255, 255),
                 180, screenHeight - 42,
                 0, ("Life: " + to_string(lifeAttempts)).c_str());

    al_draw_text(font, al_map_rgb(255, 255, 255),
                 530, screenHeight - 42,
                 0, ("Timer: " + to_string(int(timeLeft))).c_str());

    al_draw_text(font, al_map_rgb(255, 255, 255),
                 880, screenHeight - 42,
                 0, ("Score: " + to_string(score)).c_str());

    string cityName = (city != nullptr) ? city->getName() : "N/A";
    al_draw_text(font, al_map_rgb(255, 255, 255),
                 1240, screenHeight - 42,
                 0, ("City: " + cityName).c_str());
}

void gameWon() {
    isGameOver = true;

    auto gameOverFont = al_load_font("/Users/iobruno/Vault/github/allegro-pinpoint/assets/fonts/Arcade_Interlaced.ttf",
                                     64, 0);
    int xCenteredLabel = (screenWidth / 2) - 500;
    int yCenteredLabel = (screenHeight / 2) - 100;

    al_draw_text(gameOverFont, al_map_rgb(255, 255, 255),
                 xCenteredLabel, yCenteredLabel, 0, ("CONGRATULATIONS"));

    al_draw_text(font, al_map_rgb(255, 255, 255),
                 xCenteredLabel, yCenteredLabel + 150,
                 0, ("Press any key to quit the game"));

    drawHUD();
    al_draw_circle(float(mouseX), float(mouseY), 10, whiteBgColor, 5);
    al_flip_display();
    al_clear_to_color(blackBgColor);
}

void gameOver() {
    isGameOver = true;
    timeLeft = 0;

    auto gameOverFont = al_load_font("/Users/iobruno/Vault/github/allegro-pinpoint/assets/fonts/Arcade_Interlaced.ttf",
                                     64, 0);
    int xCenteredLabel = (screenWidth / 2) - 250;
    int yCenteredLabel = (screenHeight / 2) - 100;

    al_draw_text(gameOverFont, al_map_rgb(255, 255, 255),
                 xCenteredLabel, yCenteredLabel, 0, ("GAME OVER"));

    al_draw_text(font, al_map_rgb(255, 255, 255),
                 xCenteredLabel - 200, yCenteredLabel + 150,
                 0, ("Press any key to quit the game"));

    drawHUD();
    al_draw_circle(float(mouseX), float(mouseY), 10, whiteBgColor, 5);
    al_flip_display();
    al_clear_to_color(blackBgColor);
}

double computeSecsLeft() {
    return timePerAttempt - (al_get_time() - startTime);
}

double computeAccuracyScore(double distanceFromTarget) {
    double accuracyScore = 1000;
    if (distanceFromTarget <= 50) {
        accuracyScore -= 0;
    } else if (distanceFromTarget <= 100) {
        accuracyScore *= 0.75;
    } else if (distanceFromTarget <= 150) {
        accuracyScore *= 0.50;
    } else if (distanceFromTarget <= 200) {
        accuracyScore *= 0.25;
    } else {
        accuracyScore *= 0;
        lifeAttempts -= 1; // Player wasn't even trying
    }
    return accuracyScore;
}

double computeTimeBonus() {
    double timeBonusFactor = 0.05;
    return timeBonusFactor * int(timeLeft);
}

int computeScore(double distanceFromTarget) {
    double accScore = computeAccuracyScore(distanceFromTarget);
    double multiplier = computeTimeBonus();
    return int((accScore * (1 + multiplier)) / 10);
}

void destroy() {
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(bgImage);
    al_destroy_sample(bgMusic);
    al_destroy_sample_instance(bgMusicInstance);
    al_destroy_font(font);
    exit(0);
}
