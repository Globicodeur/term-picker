#include "Application.hpp"

#include "options/Options.hpp"

#include "curses/Display.hpp"

#include "data/Collection.hpp"

Application::Application(int argc, char **argv) {
    Options::parseFromCommandLine(argc, argv);

    if (Options::items.empty()) {
        std::cerr << "Nothing to pick" << std::endl;
        exit(1);
    }
}

void Application::run() {
    Collection collection;
    bool running = true;

    auto quit = [&] {
        curses::Display::quit();
        if (!Options::no_echo)
            collection.show();
        if (!Options::command.empty())
            collection.execute();
        running = false;
    };

    auto select = [&] {
        collection.select();
        if (!Options::multiple)
            quit();
    };

    auto resize = [] {
        curses::Display::instance().updateHeight();
    };

    while (running) {
        curses::Display::show(collection);

        switch (getch()) {
            case 'q':        quit();                        break;
            case ' ':
            case '\n':       select();                      break;
            case KEY_UP:     collection.prev();             break;
            case KEY_DOWN:   collection.next();             break;
            case KEY_RIGHT:  collection.expand();           break;
            case 'e':        collection.expandRecursive();  break;
            case KEY_RESIZE: resize();                      break;
            default:                                        break;
        }

    }
}
