#ifndef START_MENU_C
#define START_MENU_C

// #include "../common.c"

void __start_login() {
    clearscreen();

    // login code

    await_enter();
}

void showstart_menu() {
    printf(
        "Sample Menu\n"
        "\n"
        "1. Login\n"
        "1. Help\n"
        "3. Exit\n");

    int choice = scan_number(">> ");
    switch (choice) {
        case 1:
            // pick first choice
            __start_login();
            break;
        case 2:
            // pick second choice
            break;
        case 3:
            // pick third choice
            break;
    }
}

#endif  // ! START_MENU_C