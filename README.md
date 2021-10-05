# DA GardeningSystem
DA Bernd Hatzinger, Lukas Zrout<br>
Datenbank: https://diplomarbeiten.berufsbildendeschulen.at/

## Blockschaltbild
<img src="./Aufgabenstellung/SmartGardeningSystem_V3.png">

#### Neue Strukturänderungen (21.08.2021):
- Das Ventilmodul wird mit Niederspannungskabel versorgt.
- Die Sensorstation ist eine Box, in der sich Antenne, Prozessor befindet. 
Der Bodenfeuchtigkeitssensor wird mit einem Kabel an die Box angeschlossen. 
Zusätzlich kann man ein Niederspannungskabel anstecken (optional). Oder es wird ein Solarpanelpox (mit Akku un el. Schaltung) mittels eines Kabel angeschlossen.
- Die Hauptstation wird ebenfalls mit einem Kabel versorgt. Eigene box für Hautpstation notwendig? (Nur raspberry pi...)
- Auf jedem Modul/Station befindet sich ein Knopf zum Zurücksetzen oder erstmaligen Verbinden.

## Einteilung
#### Bernd
- Handyapp
- Base Station
#### Lukas
- Branch Station
- Website (Konfiguration/Steuerung)

# Ziele
## Ende Ferien:
#### Bernd
- Anfang Handy App
- Antenne ansteuern
- Konfigurations AP
#### Lukas
- Kommunikation zur Base-Station
- Wertanfragelogik
- ??
- Ansteuerung des Antennenmoduls via STM31F103RB
- Grundlegender Schaltplan der Branch Station (evtl. PCB)

# Used Libraries
CC1101 Top: https://github.com/LSatan/SmartRC-CC1101-Driver-Lib<br>
CC1101 Simplified: https://github.com/simonmonk/CC1101_arduino<br>
CC1101 Raspberry: https://github.com/SpaceTeddy/CC1101<br>

## Lukas
https://www.snapeda.com/parts/STM32F103RB/STMicroelectronics/view-part/596771/?ref=search&t=stm32f103rb
https://www.snapeda.com/parts/STM32F103C8T6/STMicroelectronics/view-part/?company=HTL+Hollabrunn&amp;welcome=home

ICs:
Referenz System Solar Laden: https://wiki.dfrobot.com/Solar_Power_Manager_5V_SKU__DFR0559#More_Documents
Davon IC bsp. https://www.analog.com/media/en/technical-documentation/data-sheets/3652fe.pdf
Schaltplan: https://raw.githubusercontent.com/DFRobot/Wiki/master/Solar%20Power%20Manager%20Series/DFR0535/res/DFR0535%20(V1.0)%20Schematic.pdf

evtl. https://www.ti.com/lit/ds/symlink/bq24210.pdf?ts=1629374828288&ref_url=https%253A%252F%252Fwww.google.com%252F
## Bernd
REST API Template: ~~https://marketplace.visualstudio.com/items?itemName=anasoft-code.ApiNcore3&ssr=false#overview~~<br>
                   https://marketplace.visualstudio.com/items?itemName=anasoft-code.RestApiN<br>
Worker Service: https://www.stevejgordon.co.uk/what-are-dotnet-worker-services<br>
API-Anleitung: https://docs.microsoft.com/en-us/aspnet/web-api/overview/older-versions/build-restful-apis-with-aspnet-web-api<br>
Mobile App: https://github.com/sthewissen/Xamarin.Forms.PancakeView<br>
Elements: https://editor.method.ac/<br>
Icons: https://www.iconsdb.com/white-icons/info-2-icon.html<br>
App Release: https://docs.microsoft.com/en-us/xamarin/android/deploy-test/release-prep/?tabs=windows<br>
Illustrations: https://undraw.co/illustrations<br>
DTOs: https://softwareengineering.stackexchange.com/questions/373284/what-is-the-use-of-dto-instead-of-entity<br><br>
https://ahorasomos.izertis.com/solidgear/en/xamarin-diaries-offline-by-default/<br>
SaveObjectsInFile: https://stackoverflow.com/questions/6115721/how-to-save-restore-serializable-object-to-from-file<br>
StoringPasswords (Hashes): https://medium.com/dealeron-dev/storing-passwords-in-net-core-3de29a3da4d2<br>
JSON Web Token in ASP.NET: https://www.c-sharpcorner.com/article/jwt-json-web-token-authentication-in-asp-net-core/<br>
Temporäre arbeitslinks:<br>
- https://github.com/mackayn/MVVM-Light-Sample/blob/master/XamFormsTestApp/App.xaml.cs
- https://stackoverflow.com/questions/13795596/how-to-use-mvvmlight-simpleioc
- MVVM in action: https://github.com/mackayn/CrudSample
