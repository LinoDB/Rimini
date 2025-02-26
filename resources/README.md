# Legend for the structures of the csv files

Each field consists of four sides with a female/male lower/upper body. There are
two additional umbrella cards serving as a joker. Each side of each card is
registered in the csv files with the following code:

| Body part            | Code |
| -------------------- | ---- |
| Female lower body    | FL   |
| Female upper body    | FU   |
| Male lower body      | ML   |
| Male upper body      | MU   |
| Umbrella/_undefined_ | N    |


## cards.csv

Free cards that can be placed on the field. There are 72 cards in total, their
respective number is found in the first column (_id_). The other columns
represent the body part found on the respective side.

| id  | left | up  | right | down |
| --- | ---- | --- | ----- | ---- |
| 1   | FL   | FL  | FL    | FL   |
| 2   | FL   | FL  | FL    | ML   |
| 3   | FL   | FL  | MU    | MU   |
| 4   | FL   | FU  | FL    | MU   |
| ... | ...  | ... | ...   | ...  |
| 72  | N    | N   | N     | N    |


## field.csv

Field with 9 fixed cards on which the free cards can be placed. The field is of
size 9x9, with an additional body part for each row/column along the border.
This leads to a 11x11 field, indexed from 0 to 10, with a fixed border that only
has one defined side (3 sides of _N_). In the csv file, each fixed card/border
is defined using it's position on the grid. The all-_N_ edges _[ (0, 0), (0,
10), (10, 0), (10, 10) ]_ don't need to be defined.

| row | col | left | up  | right | down |
| --- | --- | ---- | --- | ----- | ---- |
| 0   | 1   | N    | N   | N     | FU   |
| 0   | 2   | N    | N   | N     | FU   |
| ... | ... | ...  | ... | ...   | ...  |
| 0   | 9   | N    | N   | N     | MU   |
| ... | ... | ...  | ... | ...   | ...  |
| 1   | 0   | N    | N   | ML    | N    |
| 1   | 10  | FL   | N   | N     | N    |
| 2   | 0   | N    | N   | ML    | N    |
| 2   | 5   | FU   | FU  | FU    | FU   |
| 2   | 10  | FL   | N   | N     | N    |
| 3   | 0   | N    | N   | MU    | N    |
| ... | ... | ...  | ... | ...   | ...  |
| 9   | 0   | N    | N   | MU    | N    |
| 9   | 10  | ML   | N   | N     | N    |
| ... | ... | ...  | ... | ...   | ...  |
| 10  | 1   | N    | FL  | N     | N    |
| 10  | 2   | N    | ML  | N     | N    |
| ... | ... | ...  | ... | ...   | ...  |
| 10  | 9   | N    | ML  | N     | N    |
