# NuPhy Air75 V2 ANSI - Custom QMK Firmware

*NuPhy Air75 V2 is a standard 84 key keyboard.*
![NuPhy Air75 V2](https://bit.ly/486pSkN)

---
<!-- INIZIO NUOVA DOCUMNETAZIONE -->

## 📌 Info generali

Questa configurazione è basata su QMK per la tastiera **Nuphy Air75 V2 (ANSI Layout)**

### Caratteristiche principali

- Layout 75% ANSU
- Supporto QMK/VIA
- RGB per-key + side LED
- Multi-layer (Mac / Windows)
- Supporto macro avanzate
- Personalizzazione completa della keymap
- Quick Guide/Poster [download](https://cdn.shopify.com/s/files/1/0268/7297/1373/files/NuPhy_Air75_V2_Quick_Guide.pdf?v=1696498123)

### Struttura del progetto

```text
keyboards/nuphy/air75_v2/ansi/
```

## ⚙️ Installazione nuova configurazione 

### 🔨 Creazione file `.bin`

utilizzando il software QMK SYS:
1. Entra nella directory `qmk_firmware` dove è salvato il codice, in questo caso:
   ```
   Documents/GitHub/qmk_firmware
   ```

2. Per creare il file `.bin` utilizzando la keymap personalizzata:
   ```
   make nuphy/air75_v2/ansi:<NOME KEYMAP>
   ```

   👉 In questo caso usiamo la keymap `gianlu`, quindi il comando da esewguire sarà questo:
   ```
   make nuphy/air75_v2/ansi:gianlu
   ```

> [!NOTE]
> See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).


### ⚡ Flash firmware 

#### Bootload

Enter the bootloader in one way:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard

Dopo aver collegato la tastiera in modalità bootload, con il software QMK toolbox:

1. Apri il file `.bin` buildato con la procedura sopra dalla cartella `qmk_firmware`
2. Eseguire il flash del firmware sulla tastiera
3. Attendi il buon esito del flash
4. Scollega e ricollega la tastiera per uscire dal bootload e iniziare ad utilizzare la tastiera con il nuovo firmware caricato

## 🎛 Configurazione

### 🧠 Layers

| Layer | Descrizione |
| :---- | :---------- |



<!-- FINE NUOVA DOCUMNETAZIONE -->
---

* Keyboard Maintainer: [nuphy](https://github.com/nuphy-src)
* Hardware Supported: NuPhy Air75 V2 PCB
* Hardware Availability: Private
* Quick Guide/Poster [download](https://cdn.shopify.com/s/files/1/0268/7297/1373/files/NuPhy_Air75_V2_Quick_Guide.pdf?v=1696498123)

<!-- aggiunta mia, da revisionare -->

## Compilare il tuo firmware

### 1. Creare la build del file .bin della tua configurazione

Utilizzando lo strumento QMK SYS

### 2. Flashare la build sulla tastiera

### Parte vecchia

Per creare il file .bin con la tua configurazione deve essere dato il comando `make` dalla root del progetto.

#### 🔎 Controllo immediato:
Nel terminale:
```
pwd
```
Deve restituire qualcosa come:
```
.../qmk_firmware
```
Se vedi invece:
```
.../qmk_firmware/keyboard/...
```
❌ Vuol dire che sei troppo dentro.

👉 Torna quindi alla root:
```
cd ~/qmk_firmware
```

---

### ✅Comando `make`
Dalla root di `qmk_firmware`:
```
make nuphy/air75_v2/ansi:<NOME KEYMAP>
```
In questo caso la directory della keymap si chiama `gianlu`, quindi dai questo comando:
```
make nuphy/air75_v2/ansi:gianlu
```
📌Non:
* `make ansi:gianlu`
* `make air75_v2:gianlu`
* `make keyboards/...`

👉 QMK vuole il path relativo a `keyboards/`

### Esempi
<!-- fine aggiunta mia, da revisionare -->

Make example for this keyboard (after setting up your build environment):

    make nuphy/air75_v2/ansi:default

Flashing example for this keyboard:

    make nuphy/air75_v2/ansi:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in one way:

* **Bootmagic reset**: Hold down the key at (0,0) in the matrix (usually the top left key or Escape) and plug in the keyboard

<!-- Parte aggiunta ad me -->
<!--  TODO: valutare di spostare la parte sottostante in un readme separato da mettere in /keymaps -->

## My personal configuration

Le macro vengono create seguendo una particolare denominazione.

"MA_<1° lettera layer><3 lettere riconoscitive>"

Questo per non scombinare la griglia precreata che rappresenta la tastiera in ogni layer

### Layers

* Layer 2 -> Win layer
* Layer 3 -> Fn Win layer
* Layer 4 -> Layer 4 
* Layer 5 -> Outlook layer
* Layer 6 -> Ticket layer


#### Win layer

#### Fn Win layer

#### Layer 4

#### 📧 Outlook layer

Layer dedicato alle macro di testo 

<!-- TODO: aggiungere successivamente il colore del tasto una volta settato -->
| Key | Macro name | Text                                                              | String code                                                          | Key color      | Descrizione                     |
| :-: | :--------- | :----------                                                       | :-------------                                                       | :------------- | :----------                     |
|  1  | MA_OBNG    | "Buongiorno,"                                                     | `Buongiorno\n`                                                       | Green          | Saluto inizio mail              |
|  Q  | MA_OBNS    | "Buonasera,"                                                      | `Buonasera\n`                                                        | Green          | Saluto inizio mail              |
|  2  | MA_OCDC    | "Come da contato "                                                | `Come da contatto `                                                  | Yellow         | testo di chiamata               |
|  3  | MA_OJAB    | "si notifica la seguente fase in abend"                           | `si notifica la seguente fase in abend\n\n`                          | Green          | notifica abend singolo          |
|  E  | MA_OJAS    | "si notificano le seguenti fasi in abend"                         | `si notificano le seguenti fasi in abend\n\n`                        | Green          | notifica abend multiplo         |
|  4  | MA_OCOF    | "si notifica il ritardo del cut-off in oggetto al seguente CP"    | `si notifica il ritardo del cut-off in oggetto al seguente CP\n\n`   | Blue           | notifica cut-off singolo        |
|  R  | MA_OCOS    | "si notifica il ritardo dei seguenti cut-off ai CP indicati"      | `si notifica il ritardo dei seguenti cut-off ai CP indicati\n\n`     | Blue           | notifica cut-off multiplo       |
|  5  | MA_OBCH    | "si notifica il ritardo del giro in oggetto al seguente CP"       | `si notifica il ritardo del giro in oggetto al seguente CP\n\n`      | Purple         | notifica giro singolo           |
|  T  | MA_OBCS    | "si notifica il ritardo dei seguenti giri ai CP indicati"         | `si notifica il ritardo dei seguenti giri ai CP indicati\n\n`        | Purple         | notifica giri multipli          |
<!-- |     |            |                                                                   |                                                                      |                                 | -->

#### 🎟️ Ticket layer

Layer dedicato alle macro di testo da inserire sul portale di ticketing.

Layout attivabile con la pressione di Fn + P

Elenco macro di testo

<!-- TODO: aggiungere successivamente il colore del tasto una volta settato -->
| Key | Macro name | Text                                                              | String code                                                          | Key color      | Descrizione                     |
| :-: | :--------- | :----------                                                       | :-------------                                                       | :------------- | :----------                     |
|  1  | MA_TRES    | "Restartato"                                                      | `Restartato`                                                         | Green          | Testo                           |
|  Q  | MA_TREP    | "Restartato come da prosa"                                        | `Restartato come da prosa`                                           | Green          | Testo                           |
|  2  | MA_TNOT    | "Notificato"                                                      | `Notificato`                                                         | Green          | Testo                           |
|  W  | MA_TNOR    | "Notificato a reperibile"                                         | `Notificato a reperibile`                                            | Green          | Testo                           |
|  3  | MA_TNRR    | "Notificato a reperibile e restartato su sua richiesta"           | `Notificato a reperibile e restartato su sua richiesta`              | Green          | Testo                           |
|  G  | MA_TFGC    | "Fase da completare già in complete, fase completata"             | `Fase da completare già in complete, fase completata`                | Green          | Testo                           |
