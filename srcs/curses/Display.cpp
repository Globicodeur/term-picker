#include "Display.hpp"

#include "data/Collection.hpp"

namespace curses {

    Display::Display():
        lowerBound { 0 } {

        setupTerm();
        raw();
        noecho();
        keypad(stdscr, 1);
        curs_set(0);
    }

    Display::~Display() {
        if (!isendwin())
            quit();
    }

    Display & Display::instance() {
        static Display display;
        return display;
    }

    void Display::show(const Collection & collection) {
        auto & self = instance();

        int currentIndex = collection.currentIndex();
        int displayedIndex = 0;
        for (int i = 0; i < currentIndex; ++i)
            displayedIndex += collection.items()[i].get().visible;

        clear();
        if (displayedIndex >= self.lowerBound + self.height)
            self.lowerBound = displayedIndex - self.height + 1;
        else if (displayedIndex < self.lowerBound)
            self.lowerBound = displayedIndex;
        int maxSize = collection.items().size();
        int printed = 0;
        for (int i = self.lowerBound; printed < self.height && i < maxSize; ++i) {
            auto item = collection.items()[i].get();
            if (item.visible) {
                showItem(item, i == currentIndex);
                ++printed;
            }
        }
        refresh();
    }

    void Display::quit() {
        endwin();
        delscreen(instance().screen_);
    }

    void Display::updateHeight() {
        int _;
        getmaxyx(stdscr, height, _);
    }

    void Display::showItem(const Item & item, bool current) {
        auto line = std::string(item.level * 3, ' ') + "|--" + item.name + "\n";
        if (current || item.selected)
            attron(A_REVERSE);
        printw(line.c_str());
        if (current || item.selected)
            attroff(A_REVERSE);
    }

    void Display::setupTerm() {
        outFile = isatty(STDOUT_FILENO) ? stdout : fopen("/dev/tty", "w");
        inFile = isatty(STDIN_FILENO) ? stdin : fopen("/dev/tty", "r");

        screen_ = newterm(nullptr, outFile, inFile);
        updateHeight();
    }

}
