
import testdata
from  usercf import UserCF
import random
from tqdm import tqdm

def get_data():
    user_movie_records = list(testdata.readUserMovieRecord())
    random.shuffle(user_movie_records)
    x = int(len(user_movie_records)*0.95)
    train_records = user_movie_records[0:x]
    test_records = user_movie_records[x:]

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
        print(k)
        recommendation = {}
        for u in tqdm(test_users_data):
           recommendation[u] = cf.recommend(u, k,10)
        reco[k] = recommendation
    return reco


def recommend_by_random(train_movie_user_dict, test_users_data):
    movies = [k for k in train_movie_user_dict]
    recommendation = {}
    for u in tqdm(test_users_data):
        random.shuffle(movies)
        recommendation[u] = movies[0:10]
    return recommendation

if __name__ == '__main__':
    eval_info = ""
    (train_movie_user_dict, test_users_data) = get_data()
    ##Random
    recommendation = recommend_by_random(train_movie_user_dict, test_users_data)
    (precision, recall) = evaluate(test_users_data, recommendation)
    eval_info = "random P,R(%f, %f)"%(precision, recall) 
    print(eval_info)

    ##user CF
    recos = recommend_by_user_cf(train_movie_user_dict, test_users_data, [2,3,4,5,6,7])
    for rec in recos:
        eval_info += "\n"
        (precision, recall) = evaluate(test_users_data, recos[rec])
        eval_info +=  "userCF k(%d), P,R(%f, %f)"%(rec, precision, recall) 
    print(eval_info)
    