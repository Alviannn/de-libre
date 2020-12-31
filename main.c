#include "common.h"
#include "menu/user.h"

int main() {
    setlocale(LC_NUMERIC, "");
    
    USER_LIST = safe_alloc(USER_LIST, 0, sizeof(user_t));
    BOOK_LIST = safe_alloc(BOOK_LIST, 0, sizeof(book_t));

    while (true) {
        // resets the main sorting point
        MAIN_BOOK_SORT = ID_SORT;

        if (CURRENT_USER == NULL) { // if user isn't logged in
            break; // todo: show login menu
        } else { // if user is logged in
            if (CURRENT_USER->isadmin)
                break; // todo: show admin menu
            else
                showuser_menu();
        }
    }

    if (USER_LIST != NULL)
        free(USER_LIST);
    if (BOOK_LIST != NULL)
        free(BOOK_LIST);

    ULENGTH = 0;
    BLENGTH = 0;

    return 0;
}