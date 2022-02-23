
# check if a string is a valid float
def check_float(potential_float):
    try:
        float(potential_float)
        return True
    except:
        return False


# given a filepath, this function returns an array containing the columns of the file
# separation character ist whitespace by default. Specify in split if otherwise
def get_data_from_file(filepath):
    file = open(filepath, 'r')

    rows = []  # contains rows from data file

    for line in file:
        line = line.strip().split() # if data is separated by something other than whitespace specify in split(__)!
        
        # skip emty lines or lines containting text (if first char is not a number)
        if len(line) != 0:
            if not check_float(line[0]):
                continue
            
        rows.append(line)

    max_line_len = max([len(row) for row in rows])  # max length of a row / max amount of columns

    colums = [[] for i in range(max_line_len)] # contains the colums from data file

    for row in rows:
        col_index = 0
        for data_point in row:
            colums[col_index].append(data_point)
            col_index += 1

    colums_as_float = [[float(i) for i in colum] for colum in colums]

    file.close()

    return colums_as_float





