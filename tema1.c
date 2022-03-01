#include <unistd.h>
#include <stdarg.h>
#include <string.h>

static int write_stdout(const char *token, int length)
{
    int rc;
    int bytes_written = 0;

    do {
        rc = write(1, token + bytes_written, length - bytes_written);
        if (rc < 0)
            return rc;

        bytes_written += rc;
    } while (bytes_written < length);

    return bytes_written;
}

// functie de inversare a elementelor unui string
void reverse_string(char *string, int length) {
    char aux;
    for (int i = 0; i < length / 2; i++) {
        aux = string[i];
        string[i] = string[length - i - 1];
        string[length - i - 1] = aux;
    }
}

void dec_to_string(int n, char *string) {
    int length = 0, sign = 1;
    // daca numarul e negativ, pastram acest lucru in
    if (n < 0) {
        sign = -1;
    }

    while (n != 0) {
        length++;
        // daca numarul e negativ, n%10 ar fi negativ, de aceea acesta se
        // inmulteste cu semnul numarului.
        string[length - 1] = sign * (n % 10) + 48;
        n/=10;
    }
    // daca numarul este negativ, se pune caracterul '-' la sfarsitul stringului
    // inainte de a fi inversat.
    if (sign == -1) {
        length++;
        string[length - 1] = '-';
    }
    string[length] = '\0';

    // se inverseaza stringul, pentru a obtine in ordine cifrele numarului.
    reverse_string(string, length);
}

// functia care creeaza un string cu cifrele unui 'unsigned int', in baza 10
void udec_to_string(unsigned int n, char *string) {
    int length = 0;

    // se pun pe rand cifrele numarului in string, in ordine inversa
    while (n != 0) {
        length++;
        string[length - 1] = n % 10 + 48;
        n/=10;
    }
    string[length] = '\0';

    // se inverseaza stringul, pentru a obtine in ordine cifrele numarului.
    reverse_string(string, length);
}

// functia care creeaza un string cu cifrele unui 'unsigned int', in baza 16
void xdec_to_string(unsigned int n, char *string) {
    int length = 0;

    // se transforma numarul in baza 16, punand resturile impartirilor la 16,
    // pe rand, in string
    while (n != 0) {
        length++;
        // daca restul impartirii este >= 10, se pun in string literele
        // asociate(A, B etc.)
        if (n % 16 > 9) {
            string[length - 1] = n % 16 + 87;
        } else {
            string[length - 1] = n % 16 + 48;
        }
        n/=16;
    }
    string[length] = '\0';

    // se inverseaza stringul, pentru a obtine in ordine cifrele numarului.
    reverse_string(string, length);
}



int iocla_printf(const char *format, ...)
{	
    va_list arg_list;
    va_start(arg_list, format);

    int length, bytes_written = 0, aux_int;
    char aux_char, aux_string[11];
    char *aux_char_ptr;
    const char *i = format;
    // pointerul 'token' va fi asezat la inceputul stringului
    const char *token = format;
    // se itereaza pe string caracter cu caracter
    while (*i) {
        if (*i == '%') {
            // la gasirea lui '%', se scriu toate caracterele de la token la
            // pozitia curenta
            length = i - token;
            bytes_written += write_stdout(token, length);
            i++;
            token = i + 1;

            // pentru fiecare caz in parte(mai putin primul), se citeste
            // argumentul urmator si se afiseaza
            if (*i=='%') {
                bytes_written += write_stdout(i, 1);
            } else if (*i =='c') {
                aux_char = va_arg(arg_list, int);
                bytes_written += write_stdout(&aux_char, 1);
            } else if (*i == 'd') {
                aux_int = va_arg(arg_list, int);
                dec_to_string(aux_int, aux_string);
                bytes_written += write_stdout(aux_string, strlen(aux_string));
            } else if (*i == 'u') {
                aux_int = va_arg(arg_list, int);
                udec_to_string(aux_int, aux_string);
                bytes_written += write_stdout(aux_string, strlen(aux_string));
            } else if (*i == 'x') {
                aux_int = va_arg(arg_list, int);
                xdec_to_string(aux_int, aux_string);
                bytes_written += write_stdout(aux_string, strlen(aux_string));
            } else if (*i == 's') {
                aux_char_ptr = va_arg(arg_list, char*);
                bytes_written += write_stdout(aux_char_ptr, strlen(aux_char_ptr));
            }
        }
        i++;
    }
    //pointerii token si i sunt asezati imediat dupa caracterul de dupa '%'
    length = i - token;
    bytes_written += write_stdout(token, length);

    return bytes_written;
}
