#include "Application.hpp"

#include "options/Options.hpp"

#include "curses/Display.hpp"

#include "Collection.hpp"

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);
}

void Application::run() {
    Collection collection;
    bool running = true;

    auto quit = [&] {
        curses::Display::quit();
        collection.show();
        running = false;
    };

    auto select = [&] {
        collection.select();
        if (Options::single)
            quit();
    };

    while (running) {
        curses::Display::show(collection);

        int c = getch();
        switch (c) {
            case 'q':       quit();                 break;
            case ' ':
            case '\n':      select();               break;
            case KEY_UP:    collection.prev();      break;
            case KEY_DOWN:  collection.next();      break;
            case KEY_RIGHT: collection.expand();    break;
            default:                                break;
        }

    }
}
