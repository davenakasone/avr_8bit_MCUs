module tb();
    reg mask [7:0];
    reg in_data [7:0];
    reg result [7:0];
    
    initial begin
        mask = 8'b1111_0000;
        in_data = 8'b1110_1101;
        result = mask & in_data;
        $write("\n\tmasking with AND to keep 4 MSB and eliminate 4 LSB:\n")
        $write("\ninput data:  %4b_%4b\n", in_data[7:4], in_data[3:0]);
        $write("mask:        %4b_%4b\n", mask[7:4], mask[3:0]);
        $write("----------------------------------\n");
        $write("result:       %4b_%4b\n", result[7:4], result[3:0]);


        $finish;    
    end
endmodule