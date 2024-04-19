// issu en partie de l'exemple de cppreference.com

void main() {
    int a = 10;
    int b = 5;
    int i = 0;

    // CHIFFRE_NON_NUL rule
    int chiffreNonNul = 5;

    // ENTIER_DEC rule
    int entierDec = 12345;

    // ENTIER_HEXA rule
    int entierHexa = 0x1A;

    // LETTRE_HEXA rule
    int lettreHexa = 0xE;

    // ENTIER rule
    int entier = entierDec;

    // Arithmetic operators
    int sum = a + b;
    int difference = a - b;
    int product = a * b;
    int quotient = a / b;
    int remainder = a % b;

    // Comparison operators
    bool greaterThan = a > b;
    bool lessThan = a < b;
    bool notEqual = a != b;
    bool greaterThanOrEqual = a >= b;
    bool lessThanOrEqual = a <= b;
    bool equal = a == b;

    // Logical operators
    bool logicalAnd = (a > 0) && (b > 0);
    bool logicalOr = (a > 0) || (b > 0);
    bool logicalNot = !(a > 0);

    // Bitwise operators
    int bitwiseAnd = a & b;
    int bitwiseOr = a | b;
    int bitwiseXor = a ^ b;
    int bitwiseComplement = ~a;
    int leftShift = a << 2;
    int rightShift = a >> 2;

    // Boolean type
    bool booleanVariable = true;
    bool anotherBooleanVariable = false;

    // Conditional statements
    if (booleanVariable) {
        // Code to be executed if booleanVariable is true
    } else {
        // Code to be executed if booleanVariable is false
    }

    while (booleanVariable) {
        // Code to be executed repeatedly while booleanVariable is true
    }

    for (i = 0; i < 10; i=i+1) {
        // Code to be executed repeatedly for each value of i from 0 to 9
    }

    do {
        // Code to be executed at least once, and repeatedly while booleanVariable is true
    } while (booleanVariable);

    print("CHIFFRE_NON_NUL: ", chiffreNonNul,"\n");
    print("ENTIER_DEC: ", entierDec,"\n");
    print("LETTRE_HEXA: ", lettreHexa,"\n");
    print("ENTIER_HEXA: ", entierHexa,"\n");
    print("ENTIER: ", entier,"\n");

    // Print function
    print("Boolean variable: ", booleanVariable,"\n");

    print("Sum: ", sum,"\n");
    print("Difference: ", difference,"\n");
    print("Product: ", product,"\n");
    print("Quotient: ", quotient,"\n");
    print("Remainder: ", remainder,"\n");
    print("Greater than: ", greaterThan,"\n");
    print("Less than: ", lessThan,"\n");
    print("Not equal: ", notEqual,"\n");
    print("Greater than or equal: ", greaterThanOrEqual,"\n");
    print("Less than or equal: ", lessThanOrEqual,"\n");
    print("Equal: ", equal,"\n");
    print("Logical AND: ", logicalAnd,"\n");
    print("Logical OR: ", logicalOr,"\n");
    print("Logical NOT: ", logicalNot,"\n");
    print("Bitwise AND: ", bitwiseAnd,"\n");
    print("Bitwise OR: ", bitwiseOr,"\n");
    print("Bitwise XOR: ", bitwiseXor,"\n");
    print("Bitwise complement: ", bitwiseComplement,"\n");
    print("Left shift: ", leftShift,"\n");
    print("Right shift: ", rightShift,"\n");

}