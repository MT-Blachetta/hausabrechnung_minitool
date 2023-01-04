#include<string>
#include<vector>
#include<sstream>
#include<regex>
#include<iostream>
#include<stdio.h>
#include<sys/stat.h>
#include<fstream>
#include<filesystem>

using namespace std;

string savepath;

unsigned int num_wohnung;
vector<string> wohnung; // name
vector<float> quadratmeter_wohnung; // qm
vector<unsigned int> personen_wohnung; // pn
vector<float> wasserzaehler; // wr
vector<float> stromzaehler; // st
vector<float> heizungszaehler; // bilde Summe aus Heizungszaelerwerten

//kein Heizungsverbrauch nach Raum
//vector<unsigned int> wohnung_num_raum; // rn
//vector<string> raumname;
//vector<float> raum_heizungszaehler;

// Gesamtkosten
float strompreis; // sp
float wasser_gesamtkosten; // wrk
float wasser_gesamtverbrauch; // wrv
float gas_gesamtkosten; // gsk
float gas_gesamtverbrauch; // gsv
float allgemeiner_stromverbrauch; // sak
float grundsteuer; // grk
float muellabfuhrkosten; // mrk
float wohngebaeudeversicherung; // wvk
float straßenreinigung; // sgk
float abwasserkosten; // ark
float allgemeine_stromkosten;

float wohnflaeche;
unsigned int personen_gesamt;
float heizung_gesamt;

float wasserpreis;
float gaspreis;
float gaseinheitspreis;
float grundsteuer_m2;
float allgemeine_strom_person;
float muellabfuhrkosten_person;
float wohngebaeudeversicherung_m2;
float straßenreinigung_person;
float abwasserkosten_person;


// Check string for valid format

bool checkStringA(const std::string& str) {

    std::regex pattern("sp=\\d+\\.\\d{1,4};wrk=\\d+\\.\\d{1,4};wrv=\\d+\\.\\d{1,4};gsk=\\d+\\.\\d{1,4};gsv=\\d+\\.\\d{1,4};asv=\\d+\\.\\d{1,4};grk=\\d+\\.\\d{1,4};mrk=\\d+\\.\\d{1,4};wvk=\\d+\\.\\d{1,4};sgk=\\d+\\.\\d{1,4};ark=\\d+\\.\\d{1,4}");

    return std::regex_match(str, pattern);

}

bool checkStringB(const std::string& str) {

    std::regex pattern("name=\\w+;qm=\\d+(\\.\\d{1,4})?;pn=\\d+;wr=\\d+(\\.\\d{1,4})?;st=\\d+(\\.\\d{1,4})?;hz=\\d+(\\.\\d{1,4})?");

    return std::regex_match(str, pattern);

}

void save_data(const string& save_path) {

    FILE* sf;
    errno_t err;

    if ((err = fopen_s(&sf, save_path.c_str(), "w")) != 0) {
        fprintf_s(stderr, "Datei kann nicht geoeffnet werden ('%s'): %s\n" ,save_path.c_str() ,"fatal error !");
    }
    else {  // Datei wurde geoeffnet, sf wird genutzt um in die Datei zu schreiben

        fprintf_s(sf, "sp=%.4f;wrk=%.4f;wrv=%.4f;gsk=%.4f;gsv=%.4f;asv=%.4f;grk=%.4f;mrk=%.4f;wvk=%.4f;sgk=%.4f;ark=%.4f\n", strompreis, wasser_gesamtkosten, wasser_gesamtverbrauch, gas_gesamtkosten, gas_gesamtverbrauch, allgemeiner_stromverbrauch, grundsteuer, muellabfuhrkosten, wohngebaeudeversicherung, straßenreinigung, abwasserkosten);

        for (unsigned int i = 0; i < num_wohnung; i++) {
            fprintf_s(sf, "name=%s;qm=%.4f;pn=%u;wr=%.4f;st=%.4f;hz=%.4f\n", wohnung[i].c_str(), quadratmeter_wohnung[i], personen_wohnung[i], wasserzaehler[i], stromzaehler[i], heizungszaehler[i]);
        }
        
        fclose(sf);
    }

}
void input_value(float& number, const std::string& message) {

    bool check = true;
    string eingabe;

    while (check) {

        //eingabe = "";

        try {
            char confirm = 'y';
            do {
                cout << message;
                cin >> eingabe;
                number = std::stof(eingabe);
                cout << "Eingabe: " << number << " | bestaetigen 'y', aendern 'n': ";
                cin >> confirm;
            } while (confirm == 'n');
            check = false;

        }
        catch (const std::invalid_argument& e) {
            std::cout << "Die Eingegebene Zeichenfolge ist keine gueltige Zahl ! Sie muessen den Wert nochmal korrekt angeben:\n";
            continue;
        }

    }

    return;

}

void print_general_values() {
	
    printf("\nWASSER\n");
    printf("Gesamtkosten: %.2f Euro\n", wasser_gesamtkosten);
    printf("Gesamtverbrauch: %.2f m3\n", wasser_gesamtverbrauch);
    printf("Preis pro Kubikmeter: %.2f Euro\n", wasser_gesamtkosten / wasser_gesamtverbrauch);
    printf("\nGAS\n");
    printf("Gesamtkosten: %.2f Euro\n", gas_gesamtkosten);
    printf("Gesamtverbrauch: %.2f m3\n", gas_gesamtverbrauch);
    printf("Preis pro Kubikmeter: %.2f Euro\n", gas_gesamtkosten / gas_gesamtverbrauch);
    printf("\nSTROM (allgemein)\n");
    printf("allgemeiner Stromverbrauch: %.2f kWh\n", allgemeiner_stromverbrauch);
    printf("Strompreis: %.2f Euro/kWh\n", strompreis);
    printf("Allgemeine Stromkosten: %.2f Euro\n", allgemeiner_stromverbrauch * strompreis);
    printf("Stromkosten pro Person: %.2f Euro\n", allgemeine_strom_person);
    printf("\nWeitere Nebenkosten\n");
    printf("\nGrundsteuer (gesamt): %.2f Euro\n", grundsteuer);
    printf("Grundsteuer pro quadratmeter: %.2f Euro/m2\n",grundsteuer_m2);
    printf("Muellabfuhr (gesamt): %.2f Euro\n", muellabfuhrkosten);
    printf("Muellabfuhrkosten pro Person: %.2f Euro\n", muellabfuhrkosten_person);	
    printf("Wohngebaeudeversicherung (gesamt): %.2f Euro\n", wohngebaeudeversicherung);
    printf("Wohngebaeudeversicherung pro Quadratmeter: %.2f Euro/m2\n", wohngebaeudeversicherung_m2);
    printf("Strassenreinigung (gesamt): %.2f Euro\n", straßenreinigung);
    printf("Strassenreinigung pro Person: %.2f Euro\n",straßenreinigung_person);	
    printf("Abwasserkosten (gesamt): %.2f Euro\n", abwasserkosten);
    printf("Abwasserkosten pro Person: %.2f Euro\n", abwasserkosten_person);

}


void kennzahl_abfrage() {

    //input_value(hausflaeche, "Geben sie die Flaeche des gesamten Hauses in Quadratmeter m2 an. (Beispiel: 30 oder 27.5): ");
    //cout << "Hausflaeche in m2 = " << hausflaeche << "\n";

    input_value(wasser_gesamtkosten, "Wie lauten die Gesamtkosten fuer den Wasserverbrauch des gesamten Hauses in Euro (Beispiel: 189.99): ");
    cout << "Wasser (Gesamtkosten) in Euro = " << wasser_gesamtkosten << "\n";

    input_value(wasser_gesamtverbrauch, "Wie hoch ist der Gesamtverbrauch fuer Wasser des gesamten Hauses in Kubikmeter m3 (Beispiel: 40 oder 40.5): ");
    cout << "Wasser (Gesamtverbrauch) in m3 = " << wasser_gesamtverbrauch << "\n";

    input_value(gas_gesamtkosten, "Wie hoch sind die Gesamtkosten fuer Gas des gesamten Hauses in Euro (Beispiel 279.80): ");
    cout << "Gas (Gesamtkosten) in Euro = " << gas_gesamtkosten << "\n";

    input_value(gas_gesamtverbrauch, "Wie hoch ist der gesamte Gasverbrauch des Hauses in m3 (Kubikmeter), (Beispiel: 40 oder 40.5): ");
    cout << "Gas (Gesamtverbrauch) in m3 = " << gas_gesamtverbrauch << "\n";

    input_value(strompreis, "Wie hoch sind die aktuellen Stromkosten pro kW/h in Euro (Beispiel 6.99): ");
    cout << "Strompreis pro kW/h in Euro = " << strompreis << "\n";

    input_value(allgemeiner_stromverbrauch, "Wie hoch ist der allgemeine Stromverbrauch des Hauses in Kw/h (Beispiel: 25 oder 20.5): ");
    cout << "allgemeiner Stromverbrauch in kW/h = " << allgemeiner_stromverbrauch << "\n";

    input_value(grundsteuer, "Wie hoch sind die Grundsteuer fuer das ganze Haus (Grundstueck) in Euro (Beispiel 39.99): ");
    cout << "Grundsteuer in Euro = " << grundsteuer << "\n";

    input_value(muellabfuhrkosten, "Geben Sie die Gesamtkosten fuer die Muellabfuhr in Euro an (Beispiel 39.99): ");
    cout << "Muellabfuhrkosten in Euro = " << muellabfuhrkosten << "\n";

    input_value(wohngebaeudeversicherung, "Geben Sie die Gesamtkosten fuer die Wohngebaeudeversicherung in Euro an (Beispiel 39.99): ");
    cout << "Wohngebaeudeversicherung in Euro = " << wohngebaeudeversicherung << "\n";

    input_value(straßenreinigung, "Geben Sie die Gesamtkosten fuer die Strassenreinigung in Euro an (Beispiel 39.99): ");
    cout << "Strassenreinigung in Euro = " << straßenreinigung << "\n";

    input_value(abwasserkosten, "Geben Sie die Gesamtkosten fuer Abwasser in Euro an (Beispiel 39.99): ");
    cout << "Abwasserkosten in Euro = " << abwasserkosten << "\n";


}

void wohnung_aendern(unsigned int i) {

    if (i >= num_wohnung) { 
        cout << "FEHLER: Ungueltige Eingabe. Die eingegebene Wohnungs-ID gehoehrt zu keiner existierenden Wohnung; Aktion abgebrochen" << endl;
        return;
    }

    char opt;
    cout << "Welcher Wert von Wohnung " << i << " von " << wohnung[i] << " soll geaendert werden ?" << endl;
    cout << "a: Name/Haushalt" << endl;
    cout << "b: Flaeche in m2 " << endl;
    cout << "c: Anzahl Personen" << endl;
    cout << "d: Stand des Wasserzaehlers" << endl;
    cout << "e: Stand des Stromzaehlers" << endl;
    cout << "f: Heizungen" << endl;
    cout << "g: ABBRUCH" << endl;
    cout << "\nGeben Sie den richtigen Buchstaben fuer den jeweiligen Wert an, den Sie aendern moechten: ";
    cin >> opt;

    switch(opt){

        case 'a':
            string wohnungsname;
            char confirm = 'y';
            do {
                 cout << "Geben Sie den Namen der Wohnung ein: ";
                 cin >> wohnungsname;
                 cout << "Eingabe: " << wohnungsname << " | bestaetigen 'y', aendern 'n': ";
                 cin >> confirm;
                 cout << endl;
               } while (confirm == 'n');
            wohnung[i] = wohnungsname;
            break;

        case 'b':
            input_value(quadratmeter_wohnung[i], "Geben Sie die Flaeche der Wohnung in m2 an: ");
            cout << endl;
            break;

        case 'c':
            bool check = true;
            string person_str;
            unsigned int personen;
            while (check) {
            try {
                    char confirm = 'y';
                    do {
                        cout << "Wie viele Personen wohnen im Raum ? (einfache Zahl): ";
                        cin >> person_str;
                        personen = std::stoul(person_str);
                        cout << "Eingabe: " << personen << " | bestaetigen 'y', aendern 'n': ";
                        cin >> confirm;
                        cout << endl;
                       } while (confirm == 'n');
                    check = false;
                }
            catch (const std::invalid_argument& e) {
            std::cout << "Die Eingegebene Zeichenfolge ist keine gueltige Zahl ! Sie muessen den Wert nochmal korrekt angeben:\n";
            continue;
                                                   }
                           }
            personen_wohnung[i] = personen;
            break;

        case 'd': // Stand des Wasserzaehlers
            input_value(wasserzaehler[i], "Geben Sie den Stand des Wasserzaehlers in m3 ein: ");
            cout << endl;
            break;
        case 'e': // Stromzaehler
            input_value(stromzaehler[i], "Geben Sie den Stand des Stromzaehlers in kW/h ein: ");
            cout << endl;        
            break;
        case 'f':
            float heizung = 0;
            cout << "Legen sie die Raeume der Wohnung neu an und geben den Zaehlerstand fuer jede Heizung der Wohnung ein.\n";
            char next_room = 'n';

            do {
                string raumname;
                cout << "Raum (Name): ";
                cin >> raumname;

                cout << "Geben Sie den Stand der ersten (oder einzigen) Heizung an, weitere Heizungen koennen hinzugefuegt werden";
                float tmp;
                float hz = 0;
                input_value(tmp, "\nZaehlerstand der Heizung: ");
                hz += tmp;
                cout << "Heizung (Gesamteinheiten) von " << raumname << ": " << hz << endl;

                char next = 'n';
                cout << "Weiteren Heizung Zaehler angeben ? (y/n): ";
                cin >> next;

                while (next == 'y') {
                    input_value(tmp, "\nZaehlerstand der Heizung: ");
                    hz += tmp;
                    cout << "Heizung (Gesamteinheiten) von " << raumname << ": " << hz << endl;

                    cout << "Weitere Heizung angeben ? (y/n): ";
                    cin >> next;
                                    }

                heizung += hz;
                cout << "\nNaechsten Raum hinzufuegen ? (y/n): ";
                cin >> next_room;
            } while (next_room == 'y');
            heizungszaehler[i] = heizung;
            break;

        case 'g':
            cout << "Aktion abgebrochen" << endl;
            return;
        default:
            cout << "FEHLER: Ungueltige Option, Aktion wird abgebrochen" << endl;
            return;


    }


    cout << "Wohnung wurde geaendert -->" << endl;
    cout << "Name: " << wohnung[i] << endl;
    cout << "Flaeche: " << quadratmeter_wohnung[i] << endl;
    cout << "Bewohnerzahl: " << personen_wohnung[i] << endl;
    cout << "Strom: " << stromzaehler[i] << endl;
    cout << "Wasser: " << wasserzaehler[i] << endl;
    cout << "Heizung(Gas): " << heizungszaehler[i] << endl;



}

void wohnungsdaten_abfrage() {

    string wohnungsname;
    char confirm = 'y';
    do {
        cout << "Geben Sie den Namen der Wohnung ein: ";
        cin >> wohnungsname;
        cout << "Eingabe: " << wohnungsname << " | bestaetigen 'y', aendern 'n': ";
        cin >> confirm;
        cout << endl;
    } while (confirm == 'n');

    bool check = true;
    string person_str;
    unsigned int personen;
    while (check) {

        try {
            char confirm = 'y';
            do {
                cout << "Wie viele Personen wohnen im Raum ? (einfache Zahl): ";
                cin >> person_str;
                personen = std::stoul(person_str);
                cout << "Eingabe: " << personen << " | bestaetigen 'y', aendern 'n': ";
                cin >> confirm;
                cout << endl;
            } while (confirm == 'n');
            check = false;
        }
        catch (const std::invalid_argument& e) {
            std::cout << "Die Eingegebene Zeichenfolge ist keine gueltige Zahl ! Sie muessen den Wert nochmal korrekt angeben:\n";
            continue;
        }
    }

    float quadratmeter;
    input_value(quadratmeter, "Geben Sie die Flaeche der Wohnung in m2 an: ");
    cout << endl;

    float wasserverbrauch;
    input_value(wasserverbrauch, "Geben Sie den Stand des Wasserzaehlers in m3 ein: ");
    cout << endl;

    float stromverbrauch;
    input_value(stromverbrauch, "Geben Sie den Stand des Stromzaehlers in kW/h ein: ");
    cout << endl;


    float heizung = 0;
    cout << "Als naechstes legen sie die Raeume der Wohnung an und geben den Zaehlerstand fuer jede Heizung ein.\n";
    char next_room = 'n';

    do {
        string raumname;
        cout << "Raum (Name): ";
        cin >> raumname;

        cout << "Geben Sie den Stand der ersten (oder einzigen) Heizung an, weitere Heizungen koennen hinzugefuegt werden";
        float tmp;
        float hz = 0;
        input_value(tmp, "\nZaehlerstand der Heizung: ");
        hz += tmp;
        cout << "Heizung (Gesamteinheiten) von " << raumname << ": " << hz << endl;

        char next = 'n';
        cout << "Weiteren Heizung Zaehler angeben ? (y/n): ";
        cin >> next;

        while (next == 'y') {
            input_value(tmp, "\nZaehlerstand der Heizung: ");
            hz += tmp;
            cout << "Heizung (Gesamteinheiten) von " << raumname << ": " << hz << endl;

            cout << "Weitere Heizung angeben ? (y/n): ";
            cin >> next;
        }

        heizung += hz;
        cout << "\nNaechsten Raum hinzufuegen ? (y/n): ";
        cin >> next_room;


    } while (next_room == 'y');

    cout << "\nWohnung hinzugefuegt -->" << endl;
    cout << "Name: " << wohnungsname << endl;
    cout << "Flaeche: " << quadratmeter << endl;
    cout << "Bewohnerzahl: " << personen << endl;
    cout << "Strom: " << stromverbrauch << endl;
    cout << "Wasser: " << wasserverbrauch << endl;
    cout << "Heizung(Gas): " << heizung << endl;

    wohnung.push_back(wohnungsname);
    quadratmeter_wohnung.push_back(quadratmeter);
    personen_wohnung.push_back(personen);
    wasserzaehler.push_back(wasserverbrauch);
    stromzaehler.push_back(stromverbrauch);
    heizungszaehler.push_back(heizung);

    num_wohnung++;

}


int search_backwards(string target, char seperator) {

    for (int i = target.length() - 1; i >= 0; i--) {
        if (target[i] == seperator)
        {
            return i + 1;
        }
    }

    return -1;

}

int search_forward(string target, char seperator) {


    for (int i = 0; i < target.length(); i++) {
        if (target[i] == seperator)
        {
            return i + 1;
        }
    }

    return -1;

}

vector<string> splitString(string str, char delimiter)
{
    
    vector<string> stringList;

    stringstream ss(str);

    string token;
    while (std::getline(ss, token, delimiter))
    {
        stringList.push_back(token);
    }

    return stringList;
}

void load_main_data(string& dataline) {

    vector<string> kennzahlen = splitString(dataline, ';');

    //sp
    string token = kennzahlen[0];
    int vnidx = search_backwards(token, '=');
    //string namestring = token.substr(0,vnidx-1);
    string valstring = token.substr(vnidx);
    strompreis = std::stof(valstring);

    //wrk
    token = kennzahlen[1];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    wasser_gesamtkosten = std::stof(valstring);

    //wrv
    token = kennzahlen[2];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    wasser_gesamtverbrauch = std::stof(valstring);

    //gsk
    token = kennzahlen[3];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    gas_gesamtkosten = std::stof(valstring);

    //gsv
    token = kennzahlen[4];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    gas_gesamtverbrauch = std::stof(valstring);

    //asv
    token = kennzahlen[5];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    allgemeiner_stromverbrauch = std::stof(valstring);

    //grk
    token = kennzahlen[6];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    grundsteuer = std::stof(valstring);

    //mrk
    token = kennzahlen[7];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    muellabfuhrkosten = std::stof(valstring);

    //wvk
    token = kennzahlen[8];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    wohngebaeudeversicherung = std::stof(valstring);

    //sgk
    token = kennzahlen[9];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    straßenreinigung = std::stof(valstring);

    //ark
    token = kennzahlen[10];
    vnidx = search_backwards(token, '=');
    //namestring = token.substr(0, vnidx - 1);
    valstring = token.substr(vnidx);
    abwasserkosten = std::stof(valstring);



    }



void load_home_data(string& dataline) {

    //add an apartment
    num_wohnung += 1;

    //parse general_part

    vector<string> generalInfo = splitString(dataline, ';');

    string token = generalInfo[0];
    int vnidx = search_backwards(token, '=');
    string valstring = token.substr(vnidx);
    wohnung.push_back(valstring); // name

    token = generalInfo[1];
    vnidx = search_backwards(token, '=');
    valstring = token.substr(vnidx);
    float qm = std::stof(valstring);
    quadratmeter_wohnung.push_back(qm); // qm


    token = generalInfo[2];
    vnidx = search_backwards(token, '=');
    valstring = token.substr(vnidx);
    unsigned int pn = stoul(valstring);
    personen_wohnung.push_back(pn); // pn

    token = generalInfo[3];
    vnidx = search_backwards(token, '=');
    valstring = token.substr(vnidx);
    float wr = std::stof(valstring);
    wasserzaehler.push_back(wr); // wr

    token = generalInfo[4];
    vnidx = search_backwards(token, '=');
    valstring = token.substr(vnidx);
    float st = std::stof(valstring);
    stromzaehler.push_back(st); // st
	
    token = generalInfo[5];
    vnidx = search_backwards(token, '=');
    valstring = token.substr(vnidx);
    float hz = std::stof(valstring);
    heizungszaehler.push_back(hz); // hz

}

void change_single_value() {
    
  char val_ID;

  cout << "Welche Zahl moechten sie aendern ?" << endl;
  cout << "a: Wasser Gesamtkosten in Euro" << endl;
  cout << "b: Wasser Gesamtverbrauch in m3" << endl;
  cout << "c: Gas Gesatkosten in Euro" << endl;
  cout << "d: Gas Gesamtverbrauch in m3" << endl;
  cout << "e: Strompreis pro kW/h in Euro" << endl;
  cout << "f: allgemeiner Stromverbrauch in kW/h" << endl;
  cout << "g: Grundsteuer in Euro" << endl;
  cout << "h: Muellabfuhrkosten in Euro" << endl;
  cout << "i: Wohngebaeudeversicherung (gesamt) in Euro" << endl;
  cout << "j: Strassenreinigung in Euro" << endl;
  cout << "k: Abwasserkosten in Euro" << endl;
  cout << "l: ABBRECHEN" << endl;
  cout << "\nGeben Sie den richtigen Buchstaben fuer die jeweilige Kennzahl an, die Sie aendern moechten: ";
  cin >> val_ID;
  

  switch (val_ID) {
    case 'a':
        input_value(wasser_gesamtkosten, "Wie lauten die Gesamtkosten fuer den Wasserverbrauch des gesamten Hauses in Euro (Beispiel: 189.99): ");
        cout << "Wasser (Gesamtkosten) in Euro = " << wasser_gesamtkosten << "\n";
	return;
    case 'b':
	input_value(wasser_gesamtverbrauch, "Wie hoch ist der Gesamtverbrauch fuer Wasser des gesamten Hauses in Kubikmeter m3 (Beispiel: 40 oder 40.5): ");
        cout << "Wasser (Gesamtverbrauch) in m3 = " << wasser_gesamtverbrauch << "\n";
        return;
    case 'c':
        input_value(gas_gesamtkosten, "Wie hoch sind die Gesamtkosten fuer Gas des gesamten Hauses in Euro (Beispiel 279.80): ");
        cout << "Gas (Gesamtkosten) in Euro = " << gas_gesamtkosten << "\n";		    
        return;
    case 'd':
	input_value(gas_gesamtverbrauch, "Wie hoch ist der gesamte Gasverbrauch des Hauses in m3 (Kubikmeter), (Beispiel: 40 oder 40.5): ");
        cout << "Gas (Gesamtverbrauch) in m3 = " << gas_gesamtverbrauch << "\n";
	return;
    case 'e':
        input_value(strompreis, "Wie hoch sind die aktuellen Stromkosten pro kW/h in Euro (Beispiel 6.99): ");
        cout << "Strompreis pro kW/h in Euro = " << strompreis << "\n";    
	return;
    case 'f':
        input_value(allgemeiner_stromverbrauch, "Wie hoch ist der allgemeine Stromverbrauch des Hauses in Kw/h (Beispiel: 25 oder 20.5): ");
        cout << "allgemeiner Stromverbrauch in kW/h = " << allgemeiner_stromverbrauch << "\n";
	return;
    case 'g':
        input_value(grundsteuer, "Wie hoch sind die Grundsteuer fuer das ganze Haus (Grundstück) in Euro (Beispiel 39.99): ");
        cout << "Grundsteuer in Euro = " << grundsteuer << "\n";
	return;
    case 'h':
        input_value(muellabfuhrkosten, "Geben Sie die Gesamtkosten fuer die Muellabfuhr in Euro an (Beispiel 39.99): ");
        cout << "Muellabfuhrkosten in Euro = " << muellabfuhrkosten << "\n";
	return;
    case 'i':
	input_value(wohngebaeudeversicherung, "Geben Sie die Gesamtkosten fuer die Wohngebaeudeversicherung in Euro an (Beispiel 39.99): ");
        cout << "Wohngebaeudeversicherung in Euro = " << wohngebaeudeversicherung << "\n";
	return;
    case 'j':
	input_value(straßenreinigung, "Geben Sie die Gesamtkosten fuer die Strassenreinigung in Euro an (Beispiel 39.99): ");
        cout << "Strassenreinigung in Euro = " << straßenreinigung << "\n";
	return;
    case 'k':
        input_value(abwasserkosten, "Geben Sie die Gesamtkosten fuer Abwasser in Euro an (Beispiel 39.99): ");
        cout << "Abwasserkosten in Euro = " << abwasserkosten << "\n";
        return;
    case 'l':
	    return;		  
    default:
	    cout << "Ungueltige Eingabe --> Abbruch, bitte geben sie naechstes mal einen Buchstaben zwischen a-l an." << endl;
	    return;
    
    }

}

bool compute_measures() {

    if(num_wohnung <= 0){
        cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben keine Wohnungen angelegt. Fuegen Sie mit der jeweiligen Option mindestens eine Wohnung hinzu" << endl;
        return false;
    }

    wohnflaeche = 0;
    personen_gesamt = 0;
    heizung_gesamt = 0;

    for (int i = 0; i < num_wohnung; i++) {
        wohnflaeche += quadratmeter_wohnung[i];
        personen_gesamt += personen_wohnung[i];
        heizung_gesamt += heizungszaehler[i];
    }
	
    if(wohnflaeche < 1) {
        cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben falsche Angben zur Flaeche der Wohnungen in m2 gemacht. Diese müssen zusammen mindestens = 1 ergeben !" << endl;
        cout << "Loeschen Sie alle Wohnungen und legen Sie alles neu an oder bearbeiten Sie eine bestimmte Wohnung um die Flaeche zu aendern." << endl;
	    return false;
	                    }
	
    if(personen_gesamt < 1) {
        cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben keiner Wohnung Personen hinzugefuegt. Loeschen Sie alle Wohnungen und legen Sie alles neu an oder bearbeiten Sie eine bestimmte Wohnung um eine Personenzahl von mindestens 1 anzugeben" << endl;
	    return false;    
                            }
	
    if(heizung_gesamt < 1) { 
        cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben keiner Wohnung Heizungszaehler hinzugefuegt. Loeschen Sie alle Wohnungen und legen Sie alles neu an oder bearbeiten Sie eine bestimmte Wohnung um einen korrekten Heizungswert anzugeben" << endl;
        return false;
                           }


    //allgemeine Stromkosten
    allgemeine_stromkosten = allgemeiner_stromverbrauch * strompreis;

    //wasserpreis
    if(wasser_gesamtverbrauch < 1) {
        cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben keinen Wert groesser 1 für den Wasser Gesamtverbrauch angegeben. Aendern Sie die Kennzahl mit der jeweiligen Option" << endl;
        return false;
    }
    wasserpreis = wasser_gesamtkosten / wasser_gesamtverbrauch;

    //gaspreis
    if(gas_gesamtverbrauch < 1) {
        cout << "FEHLER: Die Aktion kann nicht durchgefuehrt werden\nSie haben keinen Wert groesser 1 für den Gas Gesamtverbrauch angegeben. Aendern Sie die Kennzahl mit der jeweiligen Option" << endl;
        return false;
    }
    gaspreis = gas_gesamtkosten / gas_gesamtverbrauch;
	
    gaseinheitspreis = gas_gesamtkosten / heizung_gesamt;

    //allgemeinstrom_person
    allgemeine_strom_person = allgemeine_stromkosten / personen_gesamt;

    //grundsteuer_m2
    grundsteuer_m2 = grundsteuer / wohnflaeche;

    //wohngebaeudeversicherung_m2
    wohngebaeudeversicherung_m2 = wohngebaeudeversicherung / wohnflaeche;

    //muellabfuhrkosten_person
    muellabfuhrkosten_person = muellabfuhrkosten / personen_gesamt;

    //straßenreinigung_person
    straßenreinigung_person = straßenreinigung / personen_gesamt;

    //abwasserkosten_person
    abwasserkosten_person = abwasserkosten / personen_gesamt;
	
    return true;


}

void wohnungen_anzeigen() {
	
    unsigned int ID = 0;

    unsigned int colsize = 13; // mindestgroesse für die Spalten der Tabelle
    //unsigned int wlen = 0;
    int rowsize = 14;
    string subline = "--------------|"; // colsize +1 +1
    string row_line = "--------------|"; // rowsize +1

    //first row head
    cout << "\n              "; // OK, rowsize 
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*s  ",colsize,wohnung[w].c_str()); } // colsize + 2 seperation
    cout << endl;

    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;
	
    //second row, ID
    cout << " ID           |";// rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { 
	    printf("%*u  |", colsize-1, ID);
	    ID++;
    }
    cout << endl;
	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;

    //third row flaeche(m2)
    cout << " Flaeche (m2) |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, quadratmeter_wohnung[w]); }
    cout << endl;

    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;


    //fourth row, personen
    cout << " Personen     |";
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*u  |", colsize-1, personen_wohnung[w]); }
    cout << endl;
    	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;
	
    //fifth row, stromzaehler
    cout << " Strom (kW/h) |";// rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, stromzaehler[w]); }
    cout << endl;
	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;
	
    //sixth row, wasserzaehler
    cout << " Wasser (m3)  |";// rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, wasserzaehler[w]); }
    cout << endl;
	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;
	
    //seventh row, heizungszaehler
    cout << " Heizung      |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, heizungszaehler[w]); }
    cout << endl;
	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;


}

void hausabrechnung() { 

    unsigned int colsize = 13; // mindestgroesse für die Spalten der Tabelle
    //unsigned int wlen = 0;
    int rowsize = 27;
    string subline = "--------------|"; // colsize +1 +1
    string row_line = "---------------------------|"; // rowsize +1
	

    //first row head
    cout << "\n                           "; // OK, rowsize 
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*s  ",colsize,wohnung[w].c_str()); } // colsize + 2 seperation
    cout << endl;

    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;
	
    // stromkosten
    cout << " Stromkosten (Euro)        |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, stromzaehler[w]*strompreis); }
    cout << endl;
	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;
	
    //second
    cout << " Wasserkosten (Euro)       |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, wasserzaehler[w]*wasserpreis); }
    cout << endl;
	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;
	
    //third	
    cout << " Heizungskosten (Euro)     |";
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, heizungszaehler[w]*gaseinheitspreis); }
    cout << endl;
	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;
	
    //third 
    cout << " allgemeine Stromkosten    |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, personen_wohnung[w]*allgemeine_strom_person); }
    cout << endl;

    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;
	
    //
    cout << " Grundsteuer (Euro)        |";// rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, quadratmeter_wohnung[w]*grundsteuer_m2); }
    cout << endl;
	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;
	
    //seventh
    cout << " Wohngebaeudeversicherung  |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, quadratmeter_wohnung[w]*wohngebaeudeversicherung_m2 ); }
    cout << endl;

    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;	

    cout << " Muellabfuhrkosten (Euro)  |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, personen_wohnung[w]*muellabfuhrkosten_person); }
    cout << endl;
	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;
	
    cout << " Strassenreinigung (Euro)  |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, personen_wohnung[w]*straßenreinigung_person); }
    cout << endl;
	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;

    cout << " Abwasserkosten (Euro)     |"; // rowsize +1
    for (unsigned int w = 0; w < num_wohnung; w++) { printf("%*.2f  |", colsize-1, personen_wohnung[w]*abwasserkosten_person); }
    cout << endl;
	
    // Unterteilung 
    cout << row_line;
    for (unsigned int w = 0; w < num_wohnung; w++) { cout << subline; }
    cout << endl;

    /*for (unsigned int w = 0; w < num_wohnung; w++) {
        wlen = wohnung[w].size();
        if (wlen > longest) { longest = wlen; }
    }*/

}

bool main_dialog() {

    char option;
    cout << "\nWaehlen Sie eine der folgenden Optionen aus:\n";
    cout << "a: Neue Wohnung anlegen\n";
    cout << "b: Eingaben und Daten speichern\n";
    cout << "c: HAUSABRECHNUNG durchfueren und anzeigen\n";
    cout << "d: gespeicherte Kennzahlen anzeigen\n";
    cout << "e: gespeicherte Wohnungsdaten anzeigen\n";
    cout << "f: neue Kennzahlen eingeben\n";
    cout << "g: Einzelne Kennzahl aendern\n";
    cout << "h: Wohnungsdaten loeschen\n";
    cout << "i: Daten einzelner Wohnung aendern\n";
    cout << "j: ENDE\n";
    cin >> option;

    switch (option) {

    case 'a':
	    cout << "\nNEUE WOHNUNG ANLEGEN , Daten werden abgefragt... " << endl;
        wohnungsdaten_abfrage();
        return true;

    case 'b':
        cout << "\nSpeichere Eingabedaten in Datei..." << endl;
        if (compute_measures()) { save_data("abrechnungsdaten.sav"); }        
        return true;
        
    case 'c':
        if (compute_measures()) { hausabrechnung(); }
        return true;
        
    case 'd':
        print_general_values();
        return true;
        
    case 'e':
        if(num_wohnung >= 1) { wohnungen_anzeigen(); }
        else {cout << "Es sind keine Wohnungen eingegeben oder gespeichert. Legen Sie zuerst Wohnungen mit der jeweiligen Option an." << endl; }
        return true;
        
    case 'f':
        kennzahl_abfrage();
        return true;
        
    case 'g':
        change_single_value();
        return true;
        
    case 'h':
        cout << "Alle Wohnungen wurden geloescht und muessen neu eingegeben werden" << endl;
        num_wohnung = 0;
        wohnung = {}; // name
        quadratmeter_wohnung = {}; // qm
        personen_wohnung = {}; // pn
        wasserzaehler = {}; // wr
        stromzaehler= {}; // st
        heizungszaehler = {}; 
        return true;
        
    case 'i':
        cout << "Verfuegbare Wohnungen:\n" << endl;
        wohnungen_anzeigen();
        unsigned int ID;
        string ID_str;        
        try{
        cout << "\nGeben Sie die jeweilige ID der Wohnung ein, dessen Werte veraendert werden sollen: "
        cin >> ID_str;
        ID = std::stoul(ID_str); 
        
        }
        catch (const std::invalid_argument& e) { cout << "Der eingegebene Wert ist keine gueltige ID als ganze Zahl. Fuehren Sie die Aktion mit korrekter Eingabe erneut durch !" << endl; }
        wohnung_aendern(ID);
        return true;

    case 'j':
        // ENDE
        return false;
    default:
        cout << "FEHLER, bitte geben Sie einen Buchstaben ein, der fuer eine gueltige Option steht !"; //<< endl;
        return true;
        //break;

    }

}



int main(int argc, char* argv[]) {

    //num_wohnung = 0;

    cout << "\nWillkommen beim Programm fuer Ihre Hausabrechnung -\nSuche nach gespeicherten Daten..." << endl;

    /* //überflüssig 
    
    string loadpath = argv[0];

    unsigned int sep_position = 0;
    for (int i = loadpath.length() - 1; i > 1; i--) {
        if (loadpath[i] == '\\' || loadpath[i] == '/')
        {
            sep_position = i + 1;
            break;
        }
    }

    string resulting_string = loadpath.substr(0, sep_position);
    savepath = resulting_string + "abrechnung.sav";
    //cout << "resulting string is " << newpath << "\n"; */

    ifstream ifile;
    ifile.open("abrechnung.sav");
    if (ifile) { // Speicherdatei existiert => Lade Daten in das Programm

        string first_line;
        cout << "Speicherdatei vorhanden, wird geladen..." << endl;
        if (getline(ifile, first_line)) {
            if (checkStringA(first_line))   { 
                load_main_data(first_line);
		    
                string line;
                while (getline(ifile, line))     {
                    if (checkStringB(line)) { load_home_data(line); }
                    else { cout << "FEHLER beim Lesen der Wohnungsdaten, eine Wohnung wird nicht hinzugefuegt"; }
                                                 }           
            
                                            }
            else { cout << "FEHLER: Die Speicherdatei ist nicht korrekt formatiert, beschaedigt oder manipuliert.\nAlle Daten muessen leider erneut eingegeben werden" << endl;
	           kennzahl_abfrage();
               print_general_values();
	           cout << "Als naechstes muessen sie mindestens eine Wohnung anlegen, bitte geben Sie die gewuenschten Daten ein >>" << endl;
		       
               char nxt = 'n';
               do{
                  wohnungsdaten_abfrage(); 
                  cout << "WEITERE WOHNUNG EINGEBN ? (y/n): "; 
                  cin >> nxt;
                 } while (nxt == 'y')

		 }
                                         }
        else { cout << "FEHLER: Die Speicherdatei ist nicht korrekt formatiert, beschaedigt oder manipuliert.\nAlle Daten muessen leider erneut eingegeben werden" << endl;
	           kennzahl_abfrage();
               print_general_values();
	           cout << "Als naechstes muessen sie mindestens eine Wohnung anlegen, bitte geben Sie die gewuenschten Daten ein >>" << endl;
               char nxt = 'n';
               do{
                  wohnungsdaten_abfrage(); 
                  cout << "WEITERE WOHNUNG EINGEBN ? (y/n): "; 
                  cin >> nxt;
                 } while (nxt == 'y')
	     }

        ifile.close();

                }

    else {
        
               cout << "Speicherdatei nicht gefunden oder umbennant, Start mit Abfrage der Abrechnungsdaten... \n";
               kennzahl_abfrage();
               print_general_values();
	           cout << "Als naechstes muessen sie mindestens eine Wohnung anlegen, bitte geben Sie die gewuenschten Daten ein >>" << endl;
               char nxt = 'n';
               do{
                  wohnungsdaten_abfrage(); 
                  cout << "WEITERE WOHNUNG EINGEBN ? (y/n): "; 
                  cin >> nxt;
                 } while (nxt == 'y')

        }

    ifile.close();
	
    bool status = true;
    while (status) { status = main_dialog(); }



    /* char save_dialog = 'y';
    cout << "Werte speichern ? (y/n): ";
    cin >> save_dialog;
    if (save_dialog == 'y') { save_values(); } */ 

    //cin.get();
    return 0;

}
