import configparser
import pandas
import matplotlib.pyplot as plt
import ast
from pathlib import Path
from scipy import linalg
import copy
import matplotlib.cm as cm
import numpy as np
import random

"""Global and common data"""
data_file_path = ""
drop_first_col = False
total_samples = 0
current_total_samples = 0
total_column = 0
feature_column = 0
start_index = 0
end_index = 0
initial_chunk_percentage = 10
features = []
target = []
sample_initial_mean = []
total_mean = []
cur_class_mean = {}  # dictionary with class as key and mean vector as value
cur_class_sample_count = {}
prev_class_mean = {}
prev_class_sample_count = {}
classes = []
max_chunk_size = 10


"""chunk variables"""
chunk_features, chunk_target = [], []
chunk_mean = []
chunk_cur_class_mean = {}
chunk_cur_class_sample_count = {}
chunk_classes = []

"""final matrices"""
s_w = []
s_b = []
s_w_inverse = []
eigen_values, eigen_vector_matrix = [], []


def initialise_var():
    """initialises matrices, vectors before populating"""
    global total_samples, total_mean, sample_initial_mean, feature_columns, s_w, s_b, classes
    sample_initial_mean = []

    if total_samples > 0:
        sample_initial_mean = [0 for x in range(feature_columns)]
        s_w = [0 for x in range(feature_columns)]
        s_b = [0 for x in range(feature_columns)]
        classes = np.unique(target)
        for class_label in classes:
            cur_class_mean[class_label] = sample_initial_mean.copy()
            cur_class_sample_count[class_label] = 0


def calculate_mean():
    """ calculates mean vector of entire class"""
    global sample_initial_mean, total_mean, total_samples, cur_class_sample_count, cur_class_mean, feature_columns
    total_mean = sample_initial_mean.copy()
    index = 0
    a = np.array(features)
    for eachrow in enumerate(a):  # iterating over rows of data and calculating sum of the values
        row = eachrow[1]
        count = 0
        class_label = target[index]
        for val in row:
            total_mean[count] += val
            cur_class_mean[class_label][count] += val
            count += 1
        cur_class_sample_count[class_label] += 1
        index += 1

    # calculating mean vector of all samples
    count = 0
    for val in total_mean:
        total_mean[count] = val/index
        count += 1

    # calculating mean vector of class samples
    for class_label, mean in cur_class_mean.items():
        count = 0
        while count < feature_columns:
            cur_class_mean[class_label][count] = mean[count] / cur_class_sample_count[class_label]
            count += 1


def multiply_list_by_num(lst, num):
    return [i * num for i in lst]


def add_lists(list1, list2):
    return np.add(list1,list2)


def divide_list_by_num(lst, num):
    return [i / num for i in lst]


def update_seq_total_mean_vector(new_sample):
    """update the total mean vector after adding one sample"""
    global total_mean, current_total_samples
    total_mean = multiply_list_by_num(total_mean, current_total_samples)
    total_mean = add_lists(total_mean, new_sample)
    total_mean = divide_list_by_num(total_mean, current_total_samples + 1)
    current_total_samples += 1


def inverse_scatter_matrix():
    global s_w, feature_columns, s_w_inverse
    s_w_inverse = linalg.inv(np.array(s_w, dtype='float'))


def solve_gen_eigen_value_prob():
    """find the eigen values of S_w_inverse*s_b"""
    global eigen_values, eigen_vector_matrix, s_b, s_w
    s_b = np.array(s_b, dtype='float')
    s_w = np.array(s_w, dtype='float')
    eigen_values, eigen_vector_matrix = linalg.eigh(s_b, s_w)
    index = np.argsort(eigen_values)[::-1]
    eigen_values = eigen_values[index]
    eigen_vector_matrix = eigen_vector_matrix[:, index]  # sort eigenvectors
    eigen_vector_matrix /= linalg.norm(eigen_vector_matrix, axis=0)

def calculate_between_class_scatter_matrix():
    """calculates between class scatter matrix"""
    global s_b
    s_b = [0 for x in range(feature_columns)]
    for class_label in classes:
        temp_deviation_vector = np.array([np.array(cur_class_mean[class_label]) - np.array(total_mean)])
        col_vector = np.array([temp_deviation_vector]).transpose()
        s_b += cur_class_sample_count[class_label] * (np.dot(col_vector, temp_deviation_vector))
    s_b = s_b.reshape(feature_columns, feature_columns)


def calculate_within_class_scatter_matrix():
    """calculates within class scatter matrix"""
    global classes, s_w
    feature_rows = np.array(features)
    index = 0
    for each_row in feature_rows:
        class_label = target[index]
        temp_deviation_vector = np.array([each_row - cur_class_mean[class_label]])
        col_vector = np.array([temp_deviation_vector]).transpose()
        s_w += (np.dot(col_vector, temp_deviation_vector))
        index += 1
    s_w = s_w.reshape(feature_columns, feature_columns)


def check_if_file_exists(filename):
    """This function checks whether the dataset file is present or not"""
    file = Path(filename)
    if not file.is_file():
        print("filename: ", filename, " doesn't exist.")
        exit()


def read_data_file():
    global features, target, feature_columns, total_samples, data
    check_if_file_exists(data_file_path)
    data = pandas.read_csv(data_file_path, header=None)
    if drop_first_col:
        # drop the 1st column as it contains the index of the row
        data.drop(columns=[0], inplace=True)
    (total_samples, feature_columns) = data.shape
    feature_columns = feature_columns - 1
    # split into features and target
    features = data.values[:, :-1]
    target = data.values[:, feature_columns]


def print_info():
    """This function prints the important information"""
    print("Data is present in file :\n ", data_file_path)
    print("total samples: \n", total_samples)
    print("total mean vector: \n", total_mean)
    print("total samples in each class: \n", cur_class_sample_count)
    print("mean samples of each class: \n", cur_class_mean)
    print("within class scatter matrix: \n", s_w)
    print("between class scatter matrix: \n", s_b)
    print("Inverse of s_w: \n", s_w_inverse)
    print("Eigen values: \n", eigen_values)
    print("Eigen vectors: \n", eigen_vector_matrix)

def read_properties():
    """parses the properties file and set the parameters"""
    global data_file_path, drop_first_col, max_chunk_size, initial_chunk_percentage
    """reading properties.ini file and storing the global variables"""
    try:
        config = configparser.ConfigParser()
        config.read('properties.ini')
        data_file_path = config['ILDA']['data_file_path']
        drop_first_col = ast.literal_eval(config['ILDA']['drop_first_col'])
        max_chunk_size = int(config['ILDA']['max_chunk_size'])
        initial_chunk_percentage = int(config['ILDA']['initial_chunk_percentage'])
    except KeyError as error:
        print("Key is not present in the file: ", error)
    except Exception as error:
        print("exception occured. Cause: ", error)


def update_sequential_mean(new_sample, new_sample_class, is_new_class):
    update_seq_total_mean_vector(new_sample)
    # if target is new class
    if is_new_class:
        cur_class_sample_count[new_sample_class] = 1
        cur_class_mean[new_sample_class] = new_sample
    else:
        prev_sample_count = prev_class_sample_count[new_sample_class]
        cur_class_sample_count[new_sample_class] += 1
        cur_class_mean[new_sample_class] = divide_list_by_num(
            add_lists(
                multiply_list_by_num(prev_class_mean[new_sample_class], prev_sample_count), new_sample),
            (prev_sample_count + 1))


def update_seq_within_class_scatter_matrix(new_sample, new_sample_class, isNewClass):
    """ This function updates the within class scatter matrix if it receives sequential data
        Before reaching this function itself, the count of class in which the sample is present is updated.
        changed the formula a bit"""
    global s_w, feature_columns, prev_class_mean, prev_class_sample_count, cur_class_mean, cur_class_sample_count
    if not isNewClass:
        temp_deviation_vector = np.array([np.array(new_sample) - np.array(prev_class_mean[new_sample_class])])
        col_vector = np.array([temp_deviation_vector]).transpose()
        prod = (prev_class_sample_count[new_sample_class] / cur_class_sample_count[new_sample_class]) * (
            np.dot(col_vector, temp_deviation_vector))
        prod = prod.reshape(feature_columns, feature_columns)
        s_w = prod + s_w


def calculate_chunk_means(chunk):
    global feature_columns, prev_class_sample_count, prev_class_mean, total_mean, \
        current_total_samples, chunk_mean, chunk_cur_class_mean, chunk_cur_class_sample_count, cur_class_sample_count, chunk_features, chunk_target, classes
    chunk_features = chunk.values[:, :-1]
    chunk_target = chunk.values[:, (feature_columns)]
    chunk_mean = sample_initial_mean.copy()
    chunk_cur_class_mean = {}
    chunk_cur_class_sample_count = {}
    chunk_classes = np.unique(chunk_target)
    for class_label in chunk_classes:
        chunk_cur_class_mean[class_label] = sample_initial_mean.copy()
        chunk_cur_class_sample_count[class_label] = 0
    # find the total mean of chunk data, class means of chunk data
    chunk_sample_cnt = 0
    a = np.array(chunk_features)
    for eachrow in enumerate(a):  # iterating over rows of data and calculating sum of the values
        row = eachrow[1]
        count = 0
        class_label = chunk_target[chunk_sample_cnt]
        for val in row:
            chunk_mean[count] += val  # Ly
            chunk_cur_class_mean[class_label][count] += val
            count += 1
        chunk_cur_class_sample_count[class_label] += 1
        chunk_sample_cnt += 1

    # calculating mean vector of all samples
    count = 0
    for val in chunk_mean:
        chunk_mean[count] /= chunk_sample_cnt
        count += 1

    # calculating mean vector of class samples
    for class_label, mean in chunk_cur_class_mean.items():
        count = 0
        while count < feature_columns:
            chunk_cur_class_mean[class_label][count] = mean[count] / chunk_cur_class_sample_count[class_label]
            count += 1

    # update previous and cur class mean and total mean
    prev_class_mean = copy.deepcopy(cur_class_mean)
    prev_class_sample_count = copy.deepcopy(cur_class_sample_count)

    total_mean = divide_list_by_num(
        add_lists(multiply_list_by_num(total_mean, current_total_samples), multiply_list_by_num(chunk_mean, chunk_sample_cnt)),
        current_total_samples + chunk_sample_cnt)
    current_total_samples += chunk_sample_cnt

    for class_label, mean in chunk_cur_class_mean.items():
        if class_label in classes:  # existing class
            cur_class_mean[class_label] = divide_list_by_num(
                add_lists(multiply_list_by_num(prev_class_mean[class_label], prev_class_sample_count[class_label]),
                          multiply_list_by_num(mean, chunk_cur_class_sample_count[class_label])),
                prev_class_sample_count[class_label] + chunk_cur_class_sample_count[class_label])
            cur_class_sample_count[class_label] += chunk_cur_class_sample_count[class_label]
        else:  # new class
            cur_class_mean[class_label] = mean
            cur_class_sample_count[class_label] = chunk_cur_class_sample_count[class_label]
            classes = np.insert(classes, 1, class_label)
            """update the previous class mean and previous sample count of new class as 0"""
            prev_class_sample_count[class_label] = 0
            prev_class_mean[class_label] = sample_initial_mean.copy()


def calculate_chunk_within_class_scatter_matrix():
    # addind D_C for each class according to equation in paper
    global s_w, prev_class_mean, prev_class_sample_count, cur_class_sample_count
    e_coef_dict = {}
    f_coef_dict = {}
    for class_label in classes:
        n_c = prev_class_sample_count.get(class_label, 0)
        l_c = chunk_cur_class_sample_count.get(class_label, 0)
        #print(class_label," n_c: ", n_c," l_c: ", l_c)
        denominator = ((n_c + l_c) ** 2)
        d_coefficient = (n_c * (l_c ** 2)) / denominator

        e_coefficient = (n_c ** 2) / denominator
        f_coefficient = (l_c * (l_c + (2 * n_c))) / denominator
        e_coef_dict[class_label] = e_coefficient
        f_coef_dict[class_label] = f_coefficient

        temp_chunk_class_mean = np.array(chunk_cur_class_mean.get(class_label, sample_initial_mean.copy()))
        temp_prev_mean = np.array(prev_class_mean.get(class_label, sample_initial_mean.copy()))
        temp_deviation_vector = np.array([temp_chunk_class_mean - temp_prev_mean])
        col_vector = np.array([temp_deviation_vector]).transpose()
        prod = d_coefficient * (np.dot(col_vector, temp_deviation_vector))
        prod = prod.reshape(feature_columns, feature_columns)
        s_w = prod + s_w

    # traverse on each chunk sample and add e_c and f_c components
    index = 0
    a = np.array(chunk_features)
    for eachrow in enumerate(a):  # iterating over rows of data and calculating sum of the values
        current_sample = np.array(eachrow[1])
        class_label = chunk_target[index]
        temp_prev_mean = np.array(prev_class_mean.get(class_label, sample_initial_mean.copy()))
        temp_chunk_class_mean = np.array(chunk_cur_class_mean.get(class_label, sample_initial_mean.copy()))

        """add E_c"""
        temp_deviation_vector = np.array([current_sample - temp_prev_mean])
        col_vector = np.array([temp_deviation_vector]).transpose()
        prod = e_coef_dict[class_label] * (np.dot(col_vector, temp_deviation_vector))
        prod = prod.reshape(feature_columns, feature_columns)
        s_w = prod + s_w

        """add F_c"""
        temp_deviation_vector = np.array([current_sample - temp_chunk_class_mean])
        col_vector = np.array([temp_deviation_vector]).transpose()
        prod = f_coef_dict[class_label] * (np.dot(col_vector, temp_deviation_vector))
        prod = prod.reshape(feature_columns, feature_columns)
        s_w = prod + s_w
        index += 1
    """done updating the within class scatter matrix"""


def chunk_ilda():
    global max_chunk_size
    chunk_size = random.randint(1, max_chunk_size)
    chunk = get_chunk(chunk_size)
    #print("cjhunk: ",chunk_size)
    if chunk is not None:
        calculate_chunk_means(chunk)
        calculate_between_class_scatter_matrix()
        calculate_chunk_within_class_scatter_matrix()
        solve_gen_eigen_value_prob()
        # reset prev values
        prev_class_sample_count, prev_class_mean = {}, {}
        return True
    else:
        return False



def sequential_ilda():
    global classes, feature_columns, prev_class_sample_count, cur_class_sample_count, cur_class_mean, prev_class_mean
    chunk = get_chunk(1)
    if chunk is not None:
        new_sample = chunk.values[:, :-1]
        new_sample_class = chunk.values[:, feature_columns][0]

        prev_class_mean = copy.deepcopy(cur_class_mean)
        prev_class_sample_count = copy.deepcopy(cur_class_sample_count)

        is_new_class = False
        if new_sample_class not in classes:
            classes = np.insert(classes, 1, new_sample_class)
            is_new_class = True
        update_sequential_mean(new_sample, new_sample_class, is_new_class)
        calculate_between_class_scatter_matrix()
        update_seq_within_class_scatter_matrix(new_sample, new_sample_class, is_new_class)
        inverse_scatter_matrix()
        solve_gen_eigen_value_prob()
        # reset prev values
        prev_class_sample_count, prev_class_mean = {}, {}
        return True
    else:
        return False

def build_classic_ilda():
    initialise_var()
    calculate_mean()
    calculate_within_class_scatter_matrix()
    calculate_between_class_scatter_matrix()
    inverse_scatter_matrix()
    solve_gen_eigen_value_prob()


def get_chunk(chunk_size):
    """TODO: read the samples of chunk size"""
    global start_index, data, end_index
    end_index = start_index + chunk_size
    chunk = []
    if end_index <= total_samples:
        chunk = data[start_index:end_index]
        start_index = end_index
    elif start_index < total_samples and end_index > total_samples:
        chunk = data[start_index:total_samples]
        start_index = total_samples
    else:
        return None
    return chunk

def get_chunk_percent(percent):
    no_samples = int((percent/100) * total_samples)
    return get_chunk(no_samples)


def transformData():
    chunk = data[0:end_index]
    features = chunk.values[:, :-1]
    target = chunk.values[:, feature_columns]
    X_new = np.dot(features, eigen_vector_matrix)
    X_new = X_new[:, :2]
    plt.figure()
    colors = cm.rainbow(np.linspace(0, 1, len(classes)))
    for color, target_name in zip(colors, classes):
        plt.scatter(X_new[target == target_name, 0], X_new[target == target_name, 1], alpha=.8, color=color,
                    label=target_name)
    plt.legend(loc='best', shadow=False, scatterpoints=1)
    plt.title('Plot for LDA')
    plt.show()


if __name__ == '__main__':
    try:
        read_properties()
        read_data_file()
        option = int(input("Select one operation\n 1. classic LDA\n 2. Sequential ILDA \n 3. Chunk ILDA\n"))
        if option is 1:

            build_classic_ilda()
        elif option in (2,3):
            chunk = get_chunk_percent(initial_chunk_percentage)
            current_total_samples = len(chunk)
            features = chunk.values[:, :-1]
            target = chunk.values[:, feature_columns]
            build_classic_ilda()

            if option is 2:
                while(sequential_ilda()):
                        if end_index == 60 or end_index == 120:
                            transformData()
            elif option is 3:
                while (chunk_ilda()):
                    transformData()
        else:
            print("Sorry! Unexpected input")
        end_index = total_samples
        print_info()
        transformData()
    except Exception as error:
        print("exception occured. Cause: ", error)
