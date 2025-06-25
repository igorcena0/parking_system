#pragma once

class AdministratorParkingu {
public:
    void panelAdmina();
private:
    bool zaloguj();
    void pokazZajeteMiejsca();
    void pokazWolneMiejsca();
    void pokazHistorieBiletow();
    void zmienStawke();
    void dodajAbonenta();
    void usunAbonenta();
};
