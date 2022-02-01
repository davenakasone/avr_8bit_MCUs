module tb();
    integer givenA;  
    reg [2:0] givenB; 
    reg [11:0] givenC;
    integer decz;
    reg [11:0] twoz;

    initial begin
        givenA = 101;
        decz = givenA;
        twoz = givenA;
        $write("\n\t\tgiven:  %d  in decimal\n", givenA);
        $write("decimal:%5d\n", decz);
        $write("binary:   %4b_%4b_%4b\n", twoz[11:8], twoz[7:4], twoz[3:0]);
        $write("hex:      %3h\n", twoz);

        givenB = 3'b101;
        decz = givenB;
        twoz = givenB;
        $write("\n\t\tgiven:  %3b  in binary\n", givenB);
        $write("decimal:%5d\n", decz);
        $write("binary:   %4b_%4b_%4b\n", twoz[11:8], twoz[7:4], twoz[3:0]);
        $write("hex:      %3h\n", twoz);

        givenC = 12'h101;
        decz = givenC;
        twoz = givenC;
        $write("\n\t\tgiven:  %3h  in hex\n", givenC);
        $write("decimal:%5d\n", decz);
        $write("binary:   %4b_%4b_%4b\n", twoz[11:8], twoz[7:4], twoz[3:0]);
        $write("hex:      %3h\n\n", twoz);
        $finish;
    end
endmodule