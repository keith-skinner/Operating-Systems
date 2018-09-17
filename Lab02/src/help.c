//Keith Skinner
//Lab 02

static char * help_message =
        "USAGE:\n"
        "\n"
            "\tsub [ -h | --fromChars -+toChars [-i inputFile] [-o outputFile] ]\n"
        "\n"
        "DESCRIPTION:\n"
        "\n"
            "\tThis utility copies text from an input stream to an output stream replacing every\n"
            "\tinstance of a specific character in fromChars with a corresponding (position-wise)\n"
            "\tcharacter from toChars. Any characters in fromChars and in toChars that do not\n"
            "\thave corresponding counterparts in the other one are ignored. If an input file is\n"
            "\tprovided, the content is read from that file; otherwise, the standard input is used.\n"
            "\tIf an output file is provided, then the modified content is written to that file;\n"
            "\totherwise, the standard output is used.\n"
        "\n"
        "OPTIONS:\n"
        "\n"
            "\t--(followed by a string without separating space)\n"
            "\tindicates all characters that will be replaced in the processed text\n"
        "\n"
            "\t-+(followed by a string without separating space)\n"
            "\t  indicates the characters that will be used to replace corresponding\n"
            "\t  (position-wise) characters from fromChars in the processed text\n"
        "\n"
            "\t-i (followed by input file name)\n"
            "\tuse the provided file as an input stream instead of standard input\n"
        "\n"
            "\t-o (followed by output file name)\n"
            "\tuse the provided file as an output stream instead of standard output\n"
        "\n"
            "\t-h prints this help message\n"
        "\n";
char * getHelpMessage() {
    return help_message;
}