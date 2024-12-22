/*
 *      PROJECT-2
 *      KAAN GÜNER
 *      220201068
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Question{
    int question_number;
    char question_body[1000];
    char option_a[500];
    char option_b[500];
    char option_c[500];
    char option_d[500];
    char answer;
};

struct QuestionBank{
    struct Question *questions;
    int total_questions;
};

void collectQuestions(const char *filename, struct QuestionBank *qb) {
    FILE *file = fopen(filename, "r");

    qb->total_questions = 0;
    char buffer[1500];
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "Q", 1) == 0) {
            qb->total_questions++;
        }
    }

    rewind(file);
    qb->questions = (struct Question *)malloc(qb->total_questions * sizeof(struct Question));

    int index = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "Q", 1) == 0) {
            struct Question *q = &qb->questions[index];
            q->question_number = index + 1;
            strncpy(q->question_body, strchr(buffer, ':') + 2, sizeof(q->question_body) - 1);

            fgets(q->option_a, sizeof(q->option_a), file);
            fgets(q->option_b, sizeof(q->option_b), file);
            fgets(q->option_c, sizeof(q->option_c), file);
            fgets(q->option_d, sizeof(q->option_d), file);

            char answer_line[16];
            fgets(answer_line, sizeof(answer_line), file);
            q->answer = answer_line[5];

            q->question_body[strcspn(q->question_body, "\n")] = '\0';
            q->option_a[strcspn(q->option_a, "\n")] = '\0';
            q->option_b[strcspn(q->option_b, "\n")] = '\0';
            q->option_c[strcspn(q->option_c, "\n")] = '\0';
            q->option_d[strcspn(q->option_d, "\n")] = '\0';

            index++;
        }
    }
    fclose(file);
}

void creatBooklets(struct QuestionBank *qb, int m, int n) {
    for (int i = 0; i < m; i++) {
        char questions_filename[20];
        char answers_filename[20];
        sprintf(questions_filename, "booklet_%d.txt", i + 1);
        sprintf(answers_filename, "answers_%d.txt", i + 1);

        FILE *q_file = fopen(questions_filename, "w");
        FILE *a_file = fopen(answers_filename, "w");

        int selected_questions[qb->total_questions];
        for (int j = 0; j < qb->total_questions; j++) {
            selected_questions[j] = j;
        }

        for (int i = qb->total_questions - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            int temp = selected_questions[i];
            selected_questions[i] = selected_questions[j];
            selected_questions[j] = temp;
        }

        for (int j = 0; j < n; j++) {
            struct Question *q = &qb->questions[selected_questions[j]];
            fprintf(q_file, "Q%d: %s\n %s\n %s\n %s\n %s\n\n", j + 1, q->question_body, q->option_a, q->option_b, q->option_c, q->option_d);
            fprintf(a_file, "Q%d: %c\n", j + 1, q->answer);
        }
        fclose(q_file);
        fclose(a_file);
    }
}

int main() {
    srand(time(NULL));

    struct QuestionBank qb;
    collectQuestions("sampleFile.txt", &qb);

    int m, n;
    printf("Enter the number of booklets: \n");
    scanf("%d", &m);
    printf("Enter the number of questions: \n");
    scanf("%d", &n);

    creatBooklets(&qb, m, n);

    printf("Complated\n");
    return 0;
}
