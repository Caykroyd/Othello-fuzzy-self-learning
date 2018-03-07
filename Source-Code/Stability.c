void Board::CalculateStability()
{
    // If the 12 corner positions are empty, we do not need to try and calculate.
    if(stableflag == false)
        return;

    int player;

    // Initiating directional stability matrices: Horizontal, Vertical, Sinister (top-left), Dexter (top-right);
    Matrix H, V, S, D;
    H.Reset();
    V.Reset();
    S.Reset();
    D.Reset();

    // Phase 1. Edges
    for(int i = 0; i < B_SIZE; i++) {
        if(config->cell[0][i] == player) {
            H.cell[0][i] = 1;
            S.cell[0][i] = 1;
            D.cell[0][i] = 1;
        }
        if(config->cell[B_SIZE-1][i] == player){
            H.cell[B_SIZE-1][i] = 1;
            S.cell[B_SIZE-1][i] = 1;
            D.cell[B_SIZE-1][i] = 1;
        }
        if(config->cell[i][0] == player) {
            V.cell[i][0] = 1;
            S.cell[i][0] = 1;
            D.cell[i][0] = 1;
        }
        if(config->cell[i][B_SIZE-1] == player){
            V.cell[i][B_SIZE-1] = 1;
            S.cell[i][B_SIZE-1] = 1;
            D.cell[i][B_SIZE-1] = 1;
        }
    }

    // Phase 2. Full Lines (we are ignoring the case of semi-full lines, which can bring partial stability)
    for(int i = 0; i < B_SIZE; i++) {
        if(LineIsFull(0,i,1,0)) {
            FillLine(0,i,1,0,H);
        }
        if(LineIsFull(0,i,1,0)) {
            FillLine(0,i,1,0,H);
        }
        if(LineIsFull(0,i,1,0)) {
            FillLine(0,i,1,0,H);
        }
        if(LineIsFull(0,i,1,0)) {
            FillLine(0,i,1,0,H);
        }
        if(LineIsFull(0,i,1,0)) {
            FillLine(0,i,1,0,H);
        }
        if(LineIsFull(0,i,1,0)) {
            FillLine(0,i,1,0,H);
        }
        if(LineIsFull(0,i,1,0)) {
            FillLine(0,i,1,0,H);
        }
        if(LineIsFull(0,i,1,0)) {
            FillLine(0,i,1,0,H);
        }
    }


}

bool Board::LineIsFull(int origin_x, int origin_y, int a, int b)
{
    for(int k=0; IsEdge( origin_x + a * b, origin_y + b * k ) == false; k++){

        if(cell[ origin_x + a * b ][ origin_y + b * k ] == 0)
            return false;

    }

    return true;
}

void Board::FillLine(int origin_x, int origin_y, int a, int b, Matrix &m)
{
    for(int k=0; IsEdge( origin_x + a * b, origin_y + b * k ) == false; k++){

        m.cell[ origin_x + a * b ][ origin_y + b * k ] = 1;

    }
}

/*
uint_64 Board::EncodeLine(int i, int j, int a, int b)
{
    /// Each position on the board can have 3 values {empty, black, white} = {0, 1, -1}.
    /// We encode a line on the board to a Ternary number.

    uint_64 code = 0;
    uint_64 base = 1;

    for(int k=0; IsEdge( i+a*b, j+b*k ) == false; k++){
        code += base * ( cell[i+a*b][j+b*k] + 1 );
        base *= 3;
    }

    return code;
}*/
