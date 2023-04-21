#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct property {
    char type[20];
    char location[50];
    int size;
    int BHK;
    float price;
};
void add_property(FILE *fp) {
    struct property prop;
    printf("Enter property type: ");
    scanf("%s", prop.type);
    printf("Enter location: ");
    scanf("%s", prop.location);
    printf("Enter size in sqft: ");
    scanf("%d", &prop.size);
    printf("Enter number of BHK: ");
    scanf("%d", &prop.BHK);
    printf("Enter price: ");
    scanf("%f", &prop.price);
    fwrite(&prop, sizeof(struct property), 1, fp);
    printf("Property added successfully.\n");
}
void display_all_properties(FILE *fp) {
    struct property prop;
    rewind(fp);
    printf("%-20s%-50s%-10s%-10s%-10s\n", "Type", "Location", "Size", "BHK", "Price");
    while (fread(&prop, sizeof(struct property), 1, fp) == 1) {
        printf("%-20s%-50s%-10d%-10d%-10.2f\n", prop.type, prop.location, prop.size, prop.BHK, prop.price);
    }
}
void search_property(FILE *fp) {
    struct property prop;
    char location[50];
    int BHK;
    float min_price, max_price;
    printf("Enter location: ");
    scanf("%s", location);
    printf("Enter minimum number of BHK: ");
    scanf("%d", &BHK);
    printf("Enter minimum price: ");
    scanf("%f", &min_price);
    printf("Enter maximum price: ");
    scanf("%f", &max_price);
    rewind(fp);
    printf("%-20s%-50s%-10s%-10s%-10s\n", "Type", "Location", "Size", "BHK", "Price");
    while (fread(&prop, sizeof(struct property), 1, fp) == 1) {
        if (strcmp(prop.location, location) == 0 && prop.BHK >= BHK && prop.price >= min_price && prop.price <= max_price) {
            printf("%-20s%-50s%-10d%-10d%-10.2f\n", prop.type, prop.location, prop.size, prop.BHK, prop.price);
        }
    }
}
void buy_sell_property(FILE *fp) {
    int choice;
    float max_price, min_area;
    struct property prop;
    rewind(fp);

    printf("Enter your maximum budget: ");
    scanf("%f", &max_price);
    printf("Enter the minimum area required: ");
    scanf("%f", &min_area);

    printf("%-5s%-20s%-50s%-10s%-10s%-10s\n","No","Type", "Location", "Size", "BHK", "Price");
    int i = 0;
    while (fread(&prop, sizeof(struct property), 1, fp) == 1) {
        if (prop.price <= max_price && prop.size >= min_area) {
            i++;
            printf("%-5d%-20s%-50s%-10d%-10d%-10.2f\n",i ,prop.type, prop.location, prop.size, prop.BHK, prop.price);
        }
    }

    printf("Enter the property number you want to buy/sell (0 to cancel): ");
    scanf("%d", &choice);

    if (choice != 0) {
        fseek(fp, (choice - 1) * sizeof(struct property), SEEK_SET);
        fread(&prop, sizeof(struct property), 1, fp);
        printf("%-20s%-50s%-10s%-10s%-10s\n", "Type", "Location", "Size", "BHK", "Price");
        printf("%-20s%-50s%-10d%-10d%-10.2f\n", prop.type, prop.location, prop.size, prop.BHK, prop.price);
        printf("Do you want to buy/sell this property? (y/n): ");
        char confirm;
        scanf(" %c", &confirm);
        if (confirm == 'y' || confirm == 'Y') {
            if (strcmp(prop.type, "flat") == 0) {
                printf("Congratulations! You have bought/sold the flat.\n");
            } else {
                printf("Congratulations! You have bought/sold the land/property.\n");
            }
            prop.price *= 1.1; // increase the price by 10%
            fseek(fp, (choice - 1) * sizeof(struct property), SEEK_SET);
            fwrite(&prop, sizeof(struct property), 1, fp);
        } else {
            printf("Transaction cancelled.\n");
        }
    } else {
        printf("Transaction cancelled.\n");
    }
}

void edit_delete_property(FILE *fp) {
    int prop_no, choice;
    struct property prop;
    char confirm;
    rewind(fp);
    printf("%-5s%-20s%-50s%-10s%-10s%-10s\n","No","Type", "Location", "Size", "BHK", "Price");
    int i = 0;
    while (fread(&prop, sizeof(struct property), 1, fp) == 1) {

            i++;
            printf("%-5d%-20s%-50s%-10d%-10d%-10.2f\n",i ,prop.type, prop.location, prop.size, prop.BHK, prop.price);

    }
    printf("Enter the property number to edit/delete: ");
    scanf("%d", &prop_no);
    fseek(fp, (prop_no - 1) * sizeof(struct property), SEEK_SET);
    fread(&prop, sizeof(struct property), 1, fp);
    printf("%-20s%-50s%-10s%-10s%-10s\n", "Type", "Location", "Size", "BHK", "Price");
    printf("%-20s%-50s%-10d%-10d%-10.2f\n", prop.type, prop.location, prop.size, prop.BHK, prop.price);
    printf("Choose an option:\n");
    printf("1. Edit\n");
    printf("2. Delete\n");
    scanf("%d", &choice);
    switch (choice) {
        case 1:
            printf("Enter new details:\n");
            printf("Enter property type: ");
            scanf("%s", prop.type);
            printf("Enter location: ");
            scanf("%s", prop.location);
            printf("Enter size in sqft: ");
            scanf("%d", &prop.size);
            printf("Enter number of BHK: ");
            scanf("%d", &prop.BHK);
            printf("Enter price: ");
            scanf("%f", &prop.price);
            fseek(fp, (prop_no - 1) * sizeof(struct property), SEEK_SET);
            fwrite(&prop, sizeof(struct property), 1, fp);
            printf("Record updated successfully.\n");
            break;
       case 2:
            printf("Are you sure you want to delete this record? (y/n): ");
            scanf(" %c", &confirm);
            char loco[50];
            int size,found = 0,bhk;
            printf("Enter location of the property:");
            scanf("%s",loco);
            printf("Enter size of property:");
            scanf("%d",&size);
            printf("Enter Bhk in that property:");
            scanf("%d",&bhk);
            rewind(fp);
            if (confirm == 'y') {
                FILE *temp = fopen("temp.dat","wb");
                while(fread(&prop,sizeof(struct property),1,fp)==1){
                if(prop.size == size && strcmp(prop.location,loco)==0 && prop.BHK == bhk){
                        printf("\nRecord is deleted.\n");
                        found++;
                    }
                else
                    {
                        fwrite(&prop,sizeof(struct property),1,temp);
                    }
                }
                fclose(fp);
                fclose(temp);
                if(found==0){
                    printf("No such property exists:\n");
                }
                else{
                    remove("property.dat");
                    rename("temp.dat","property.dat");
                    fp = fopen("property.dat","rb+");
                }
            } else {
                  printf("Deletion cancelled.\n");
            }
            break;
      default:
           printf("Invalid choice.\n");
           break;
        }
}



int main() {
    int choice;
    FILE *fp;

    fp = fopen("property.dat", "rb+");
    if (fp == NULL) {
        fp = fopen("property.dat", "wb+");
        if (fp == NULL) {
            printf("Error: Unable to create file.\n");
            exit(1);
        }
    }

    while (1) {
        printf("\nReal Estate Advisor\n");
        printf("===================\n");
        printf("1. Add New Record\n");
        printf("2. Display Available Property\n");
        printf("3. Search\n");
        printf("4. Buy/Sell\n");
        printf("5. Edit/Delete Record\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                fclose(fp);
                printf("Thank you for using the Real Estate Advisor.\n");
                exit(0);
            case 1:
                add_property(fp);
                break;
            case 2:
                display_all_properties(fp);
                break;
            case 3:
                search_property(fp);
                break;
            case 4:
                buy_sell_property(fp);
                break;
            case 5:
                edit_delete_property(fp);
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    return 0;
}
