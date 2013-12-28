/* plot data */

void main() {
    int n = 0;
    float x = 0.0;

    creat_int_dialog_entry("n", &n);
    creat_float_dialog_entry("x", &x);

    set_up_diaog("Setup", 0, 0);

    read_dialog_window();

    printf("n = %d, x = %f\n", n, x);
}
