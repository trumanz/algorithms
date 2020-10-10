
import testdata
from  usercf import UserCF
import random
from tqdm import tqdm
import logging

def getTestData():
    matrix = [
        #SF: science fiction
        #HI: history
        #SFa, SFb, HIc, HId  movie/ user
        [1,   1,   1,   1 ], #0
        [1,   1,   1,   0 ], #1
        [1,   0,   1,   0 ], #2
        [0,   1,   1,   1 ], #3
        [1,   1,   0,   0 ], #4
        #1 connected, 0 not connected
    ]
    #predict user 5
    train_records = []
    for user in range(len(matrix)):
        for movie in range(len(matrix[user])):
            if matrix[user][movie] == 1:
                train_records.append((user, movie))
    test_records = [(4,3)]
    return  train_records, test_records


def get_data():
    user_movie_records = list(testdata.readUserMovieRecord())

    random.seed(1)
    random.shuffle(user_movie_records)
    x = int(len(user_movie_records)*0.9)
    train_records = user_movie_records[0:x]
    test_records = user_movie_records[x:]
    #train_records,test_records = getTestData()
    logging.getLogger().info("train_records: " + str(len(train_records)))
    logging.getLogger().info("test_records: " + str(len(test_records)))

    train_movie_user_dict  = testdata.tranform2MovieView(train_records)
    test_users_data = testdata.tranform2UserView(test_records)
    return (train_movie_user_dict, test_users_data)

 
def evaluate(test_users_data, recommendation):
    #precision: tp/(tp+fp)
    #recall : tp/(tp+fn);   
    tp = 0
    all_real = 0
    all_predict = 0
    for user, real_movies in test_users_data.items():
        predict = set(recommendation[user]) 
        tp += len( real_movies & predict )
        all_real += len(real_movies)
        all_predict += len(predict)
    tp = tp*1.0
    precision = tp/all_predict
    recall = tp/all_real
    return (precision, recall)

def recommend_by_user_cf(train_movie_user_dict, test_users_data, klist):
    cf = UserCF()
    cf.train(train_movie_user_dict)
    reco = {}
    for k in klist:
        #print(k)
        recommendation = {}
        for u in tqdm(test_users_data):
           recommendation[u] = cf.recommend(u, k,5)
        reco[k] = recommendation
    return reco


def recommend_by_random(train_movie_user_dict, test_users_data):
    movies = [k for k in train_movie_user_dict]
    recommendation = {}
    for u in tqdm(test_users_data):
        random.shuffle(movies)
        recommendation[u] = movies[0:5]
    return recommendation

if __name__ == '__main__':
    formatter  = logging.Formatter('%(asctime)s - %(levelname)s - %(message)s')
    logging.getLogger().setLevel(logging.INFO)
    fh = logging.FileHandler(filename="./log.txt", mode='w');
    fh.setFormatter(formatter)
    logging.getLogger().addHandler(fh)


    eval_info = ""
    (train_movie_user_dict, test_users_data) = get_data()
    #Random
    recommendation = recommend_by_random(train_movie_user_dict, test_users_data)
    (precision, recall) = evaluate(test_users_data, recommendation)
    eval_info = "Random P,R(%f, %f)"%(precision, recall)
    print(eval_info)

    ##user CF
    recos = recommend_by_user_cf(train_movie_user_dict, test_users_data, range(2,7))
    for rec in recos:
        eval_info += "\n"
        (precision, recall) = evaluate(test_users_data, recos[rec])
        eval_info += "UserCF k(%d), P,R(%f, %f)"%(rec, precision, recall)
    print(eval_info)
    