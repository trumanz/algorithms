
import testdata
from  usercf import UserCF
import random

def get_data():
    user_movie_records = list(testdata.readUserMovieRecord())
    random.shuffle(user_movie_records)
    x = int(len(user_movie_records)*0.9)
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

def recommend_by_user_cf(train_movie_user_dict, test_users_data):
    test_users = [k for k in test_users_data]
    cf = UserCF()
    recommendation = cf.recommend(train_movie_user_dict,test_users)
    return recommendation

if __name__ == '__main__':
    (train_movie_user_dict, test_users_data) = get_data()
    recommendation = recommend_by_user_cf(train_movie_user_dict, test_users_data)
    print(evaluate(test_users_data, recommendation))
   