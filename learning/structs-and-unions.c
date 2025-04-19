#include <stdio.h>

typedef enum brands {
    VOLKSWAGEN,
    FORD,
    MERCEDES,
    TOYOTA,
    PEUGEOT
} Brand;

typedef enum colour  {
    RED,
    GREEN,
    BLUE,
    GREY,
    BLACK,
    WHITE
} Colour;

typedef struct car Car;
struct car {
    Brand brand;
    Colour colour;
    int second_hand;
    union {
        unsigned long mileage;
        int months_guarantee;
    } car_info;
    
    unsigned diesel: 1, airbag: 1, airconditioning: 1;
};

const char *car_brands[] = {"VOLKSWAGEN", "FORD", "MERCEDES", "TOYOTA", "PEUGEOT"};
const char *car_colours[] = {"RED", "GREEN", "BLUE", "GREY", "BLACK", "WHITE"};

void print_car(Car car) {
    printf("Car Information:\n");
    printf("  Brand: %s\n", car_brands[car.brand]);
    printf("  Colour: %s\n", car_colours[car.colour]);
    printf("  Type: %s\n", car.second_hand ? "YES" : "NO");
    if (car.second_hand) {
        printf("  Mileage: %lu\n", car.car_info.mileage);
    } else {
        printf("  Months Guarantee: %d\n", car.car_info.months_guarantee);
    }
    printf(car.diesel ? "  Fuel Type: Diesel\n" : "  Fuel Type: Petrol\n");
    printf(car.airbag ? "  Airbag: Yes\n" : "  Airbag: No\n");
    printf(car.airconditioning ? "  Air-condition: Yes\n" : "  Air-condition: No\n");
}

int main(void) {
    struct car my_car = {VOLKSWAGEN, GREEN, 1, 50000, 0, 1, 0};
    struct car another_car;

    another_car.brand = PEUGEOT;
    another_car.colour = WHITE;
    another_car.second_hand = 0;
    another_car.car_info.months_guarantee = 12;
    another_car.diesel = 0;
    another_car.airbag = 1;
    another_car.airconditioning = 1;

    print_car(my_car);

    return 0;
}
