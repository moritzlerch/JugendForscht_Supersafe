# JugendForscht_Supersafe

`Der Supersafe - elektrisch doppelt gesicherter Tresor mit Arduino`
*Von Moritz Lerch und Konrad Götz*

**KURZFASSUNG**
In unserem Projekt haben wir einen Sicherheitstresor entwickelt, der durch elektronische Hilfsmittel gesichert wird. Dies geschieht zuerst mittels eines RFID-Tags, das man an einen Sensor halten muss. Anschließend muss ein passender PIN eingegeben werden. Bei korrekter Eingabe wird ein elektromagnetisch verriegeltes Schloss geöffnet. Die Ansteuerung der Elektronik erfolgt über einen Mikrokontroller (Arduino), der mit von uns selbstentwickelter Software programmiert wurde.

**Gliederung**
1. EINLEITUNG
2. BAU
  2.1. ERSTBESCHÄFTIGUNG MIT ARDUINO
  2.2. BAU DES PROTOTYPEN / MATERIALBESCHAFFUNG (ELEKTRONIK)
  2.3. PROGRAMMIERUNG
  2.4. PLANUNG DES FINALEN TRESORS
  2.5. BAU DES EIGENTLICHEN TRESORS
3. ERGEBNISSE
4. ZUSAMMENFASSUNG / AUSBLICK
5. LITERATURVERZEICHNIS / QUELLEN
6. UNTERSTÜTZUNGSLEISTUNGEN

**1. EINLEITUNG**
Ziemlich früh war uns klar, dass unser Projekt etwas mit Technik und Informatik zu tun ha-ben sollte. Moritz Lerch war bereits mit Arduino und dessen Programmiersprache „C++“ ver-traut, weshalb es nahe lag, den soeben genannten Microcontroller in unserem Projekt zu nutzen. Anfangs planten wir eventuell Bluetooth- Kopfhörer zu bauen, aber da es diese im großen Umfang zu kaufen gibt, wollten wir uns etwas Neues ausdenken. So kamen wir auf die Idee, einen Tresor mit mehreren Verifizierungsstufen zu bauen. Als erstes sollte man ei-nen RFID-Tag (in Form einer Kreditkarte oder eines kleinen Tags, den man am Schlüsselbund trägt) an einen Sensor halten und anschließend auch noch einen vierstelligen Code einge-ben. Nur so sollte man den Tresor öffnen können. Der Schwerpunkt unserer Arbeit liegt hierbei auf der Programmierung.

**2. BAU**
***2.1. ERSTBESCHÄFTIGUNG MIT ARDUINO***
Als nächstes mussten wir uns erst mit Arduino und C++ vertraut machen, dazu verwendeten wir Material der Schule. Programmiert haben wir auf unseren Tablets, welche wir wegen der Tablet-Klasse haben. Anfangs bauten wir zum Test einen Schaltkreis, der durch Knopfdruck eine kleine Lampe zum Aufleuchten brachte und später steigerten wir uns in immer schwe-rere Bereiche hinein. Wir kauften uns einen eigenen Arduino Baukasten und begannen auch die weiteren Bauteile für den Tresor zu kaufen. Darunter ein elektronisches Schloss (Sol-enoid), ein Display (I²C – LC Display; kurz LCD), ein Tastenfeld (4x4) und ein Relais.

***2.2. BAU DES PROTOTYPEN / MATERIALBESCHAFFUNG (ELEKTRONIK)***
Nachdem alle benötigten Materialien beschafft waren, die Gesamt-Kosten für die Elektronik-bauteile betrugen hierbei circa 50 Euro, begannen wir mit dem Bau des ersten Prototypen. (Siehe Abb. 1) Der Bau war nicht besonders problematisch. Es mussten nur die Bauteile ein-zeln auf einem Breadboard platziert und mit anwendungsspezifischen Kabeln verkabelt wer-den. Die Programmierung hingegen stellte uns vor eine große Herausforderung, die wir erst nach intensiver Arbeit lösen konnten.

***2.3. PROGRAMMIERUNG***
Durch die Programmierung wird die Kommunikation zwischen den Sensoren und Aktoren, in unserm Fall angesteuert durch den Arduino-Mega, realisiert. Programmiert wurde das Ganze in der Arduino typischen Programmiersprache „C++“.
Grundlegend arbeitet das Programm mit zwei Modi. Je nachdem ob eine sogenannte Boolean-Variable, die Wahrheitswerte („true“; „false“) speichern kann, den Wert „WAHR“ oder „FALSCH“ besitzt, wird entschieden, ob nun der RFID-Sensor auf einen Chip „wartet“ oder das Tastenfeld auf eine Codeeingabe. So können einige Probleme, die beim Entwickeln des Programmcodes aufgetreten sind, vorgebeugt werden. Parallel werden alle wesentlichen Schritte mit einer zweifarbigen LED begleitet und mit einem kleinen Piezo-Lautsprecher posi-tiv bzw. negativ quittiert. Das LCD zeigt außerdem Anweisungen oder Hinweise an. Hier haben wir uns dafür entschieden, dass der Mikrocontroller mit dem Auslesen des RFID-Modules startet und anschließend der Code auf eine Tasteneingabe wartet. Wenn im ersten Modus (Auslesen des RFID-Sensors) der Sensor einen RFID- oder NFC-Tag erkennt, wird die einzigartige Kartennummer (UID) des Chips zwischengespeichert und an-schließend mit einer zuvor eingelesenen und eingespeicherten UID abgeglichen. Falls nun die aktuelle UID mit der zuvor Eingespeicherten übereinstimmt wird sie als richtig „bewertet“ und durch das Ändern der Boolean-Variable auf „false“ zum zweiten Modus gewechselt, an-dernfalls wird die ausgelesene Chipnummer als falsch bewertet und erneut die UID abgeru-fen und diese wieder als wahr bzw. falsch eingestuft. Wenn die UID also nicht überein-stimmt, wertet das Programm die Karte immer wieder als falsch. Sofern im ersten Modus die Karte „richtig“ war, wird der anschließend beschriebene, zweite Modus ausgeführt. Durch das Erstellen eines vierstelligen Platzhalters wird nun der nächste Schritt möglich. Hier wird jetzt auf einen Tastendruck am Keypad gewartet und der Platzhal-ter ausgefüllt. Nach vier eigegebenen Zeichen wird geprüft, ob der eingegebene Code dem richtigen Code entspricht, wenn ja dann wird der Tresor entsperrt, wenn nein wird in einer Variable plus eins gerechnet, dann wird wieder nach einem Code gefragt. Wenn die „Code-versuche“-Variable größer drei ist und es somit drei fehlgeschlagene Versuche gab, muss man, da der Modus durch die Boolean-Variable geändert wurde, wieder den Chip scannen. So kann „Bruteforcing“ („Erraten“ des Passwortes durch schnelles, direkt aufeinanderfolgendes Ausprobieren verschiedener Kombinationen) vermieden werden. Durch Schalten eines Relais, das beim „Versorgen“ mit einem bestimmten Signal Stromfluss zwischen zwei Pins erlaubt, werden 12V auf ein sogenanntes „Solenoid“-Schloss geschaltet, das zum Öffnen der Tür mithilfe eines Magnetfeldes bei Stromfluss einen Bolzen zurückzieht. Durch einen Reed-Kontakt, der bei Kontakt mit einem Magneten, den Stromfluss unter-bricht, kann durch eine Kombination mit einem Magneten zusätzlich noch überprüft werden, ob die Tür geöffnet ist. Wenn die Zeit, in der die Türe geöffnet ist, 1, 2 oder 3 Minuten über-schreitet, wird eine akustische Warnung ausgegeben, die sich bei zunehmender Dauer än-dert. So kann vermieden werden, dass die Tür zu lange offen steht. Wenn der Tresor wieder geschlossen wurde, wird die Boolean-Variable wieder auf „true“ ge-setzt, wodurch der erste Modus wieder von neuem beginnt.

Auf dem nachfolgenden Bild sieht man ein vereinfachtes Verlaufsdiagramm, dass den Ablauf des Programmcodes noch einmal bildlich darstellen soll:
![Image of Yaktocat](https://octodex.github.com/images/yaktocat.png)

***2.4. PLANUNG DES FINALEN TRESORS***
Nach Fertigstellung der Programmierung wurde das Erstdesign des Safes auf Papier gezeich-net und dieses anschließend in ein 3D-CAD-Programm eingefügt. Dort wurden dann noch weitere Änderungen vorgenommen und so die finalen Maße bestimmt. Die Maße betragen 30cm ∙ 20cm ∙ 20cm (B ∙ H ∙ T). Diese wurden noch in das 3D-Programm eingepflegt. Schluss-endlich ergab sich folgendes: (Wesentliche Bauteile beschriftet)
![Image of Yaktocat](https://octodex.github.com/images/yaktocat.png)

