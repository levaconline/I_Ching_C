#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// I Ching Divination - 2025. core.

typedef enum
{
    OLD_YIN = 6,
    YOUNG_YANG = 7,
    YOUNG_YIN = 8,
    OLD_YANG = 9
} Line;

// Entrophy with editation prompts
unsigned int get_user_entropy(int line, int division)
{
    char input[256];
    unsigned int hash = 0;

    const char *prompts[] = {
        "Focus on your question, then type random keys: ",
        "Consider the situation, type more keys: ",
        "Clear your mind, finish with random keys: "};

    printf("[Line %d Division %d] %s", line, division, prompts[division - 1]);
    fflush(stdout);

    if (!fgets(input, sizeof(input), stdin))
    {
        return rand();
    }

    // Create more robust hash
    for (int i = 0; input[i]; i++)
    {
        hash = (hash << 7) ^ (hash >> 11) ^ input[i];
    }
    return hash + line + division; // Ensure uniqueness per step
}

// Calculate the line value
// based on the random press enter after type unique string keys and division.
// Writes debug in stdout.
Line generate_line(int line_num)
{
    int stalks = 49;

    for (int division = 1; division <= 3; division++)
    {
        srand(get_user_entropy(line_num, division));

        int left = (rand() % (stalks - 1)) + 1;
        int right = stalks - left;

        int l_rem = left % 4;
        if (!l_rem)
            l_rem = 4;
        int r_rem = right % 4;
        if (!r_rem)
            r_rem = 4;

        stalks -= (l_rem + r_rem);

        printf("  Division %d: %2d = %2d + %2d (remainders %d + %d)\n",
               division, stalks, left, right, l_rem, r_rem);
    }

    int value = stalks / 4;
    return (value >= 6 && value <= 9) ? (Line)value : YOUNG_YANG;
}

// Main function to run the I Ching divination
// with enhanced user interaction and output formatting.
int main()
{
    printf("\nI CHING DIVINATION\n");
    printf("------------------------------------\n");
    printf("Approach this with a clear mind and a specific question.\n");
    printf("For each line, you'll be prompted three times.\n");
    printf("Type random characters each time, varying your input.\n\n");

    Line hexagram[6];
    Line transformed[6];
    int changing_lines = 0;

    // Generate main hexagram
    for (int i = 0; i < 6; i++)
    {
        printf("\n=== Generating Line %d ===\n", i + 1);
        hexagram[i] = generate_line(i + 1);
        transformed[i] = hexagram[i];

        // Identify changing lines
        if (hexagram[i] == OLD_YANG)
        {
            transformed[i] = YOUNG_YIN;
            changing_lines++;
        }
        else if (hexagram[i] == OLD_YIN)
        {
            transformed[i] = YOUNG_YANG;
            changing_lines++;
        }
    }

    // Display results
    printf("\n=== YOUR HEXAGRAM ===");
    printf("\nPrimary\t\tTransformed\tLine\n");
    printf("------------------------------------\n");

    // Basic wisualization, Symbols and names for lines.
    const char *line_symbols[4] = {
        "--x--", "-----", "-- --", "---o--"};

    const char *line_names[4] = {
        "Old Yin", "Young Yang", "Young Yin", "Old Yang"};

    for (int i = 5; i >= 0; i--)
    {
        printf("%s\t%s\tLine %d: %s",
               line_symbols[hexagram[i] - 6],
               (hexagram[i] != transformed[i]) ? line_symbols[transformed[i] - 6] : "  (same)",
               i + 1, line_names[hexagram[i] - 6]);

        if (hexagram[i] != transformed[i])
        {
            printf(" → %s", line_names[transformed[i] - 6]);
        }
        printf("\n");
    }

    // Interpretation guidance
    printf("\n=== INTERPRETATION GUIDE ===\n");
    printf("- Consult the Primary Hexagram for current situation\n");
    printf("- %d changing lines present\n", changing_lines);

    if (changing_lines > 0)
    {
        printf("- Consult the Transformed Hexagram for future development\n");
        printf("- Pay special attention to changing lines\n");
    }

    printf("\nHexagram Reference:\n");
    printf("6 = Old Yin | 7 = Young Yang | 8 = Young Yin | 9 = Old Yang\n");

    return 0;
}
