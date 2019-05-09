import requests as req
import re
import webbrowser as webb
import time 
import random
from bs4 import BeautifulSoup
from user_agent import generate_user_agent


def get_proxies():
    request = req.get('https://free-proxy-list.net/')
    soup = BeautifulSoup(request.text, 'lxml')
    table = soup.find('table', {'class': 'fpltable'})
    tbody = table.find('tbody')
    rows = tbody.find_all('tr')

    proxies = []
    for row in rows:
        tds = row.find_all('td')
        if tds[4].text.lower().strip() == 'elite proxy':
            proxies.append('%s:%s'%(tds[0].text, tds[1].text))
    return proxies

def get_links(url, headers, proxy):
    '''Возвращет словарь ссылок на товар с одной страницы'''
    url_head = 'https://market.yandex.ru'
    links = {}

    first_page = req.get(url, headers=headers, proxies=proxy)
    soup = BeautifulSoup(first_page.text, 'lxml')

    # Если капча
    if soup.find_all('input', 'form__key'):
        return 'Error'

    for line_block in soup.find_all('div', 'b-serp-item'):
        link = line_block.find('a', {'class' : 'b-link'}, href=True)['href']
        title = line_block.find('div', {'class' : 'b-serp-description__title'}).get_text()
        links.update({title : 'https://market.yandex.ru{}/spec'.format(re.findall('\/product\/[\d+]', link)[0])})

    return links

def get_spec(url, headers, proxy):
    '''Возвращает словарь с нужными характеристиками'''
    page = req.get(url, headers=headers, proxies=proxy)
    soup = BeautifulSoup(page.text, 'lxml')

    # Если капча
    if soup.find_all('img', 'form__captcha'):
        return 'Error'

    needed_spec = ['количество ядер процессора', 'частота процессора', 'объем видеопамяти', 
                    'объем жесткого диска', 'объем оперативной памяти']
    items_dict = {}
    specs = soup.findAll("dl", {"class": "n-product-spec"})

    for spec in specs:
        spec_title = spec.find('span', 'n-product-spec__name-inner').get_text().lower().strip()
        if spec_title in needed_spec:
            items_dict.update({spec_title :\
                               int(re.findall('\d+', spec.find("span", "n-product-spec__value-inner").get_text())[0])})

    return items_dict



def to_file(specs_dict):
    for key in specs_dict.keys():
        if key == 'количество ядер процессора':
            specs_dict['cpu_num'] = specs_dict.pop(key)
        elif key == 'частота процессора':
            specs_dict['cpu_freq'] = specs_dict.pop(key)
        elif key == 'объем видеопамяти':
            specs_dict['gpu_memory'] = specs_dict.pop(key)
        elif key == 'объем жесткого диска':
            specs_dict['hdd_size'] = specs_dict.pop(key)
        elif key == 'объем оперативной памяти':
            specs_dict['ram'] = specs_dict.pop(key)

    with open('output.txt', 'w') as f:
        for key in specs_dict.keys():
            f.write(key+': '+str(specs_dict[key])+'\n')

    f.close()


if __name__ == '__main__':
    specs_dict = {'количество ядер процессора' : [], 'частота процессора' : [], 'объем видеопамяти' : [],
                  'объем жесткого диска' : [], 'объем оперативной памяти' : []}

    proxies = []
    print('Getting proxies')
    for proxy in get_proxies():
        proxies.append({'http' : proxy})
    print('Done')

    headers = {'accept-encoding': 'gzip, deflate', 'user-agent': generate_user_agent(), 'connection': 'keep-alive', 'accept': '*/*'}    
    n_proxy = 0

    # Перебираем 20 страниц
    for i in range(1,22):
        url = 'https://m.market.yandex.ru/catalog/54544/list?page={}'.format(i)
        links = get_links(url, headers, proxies[n_proxy])
        print(proxies[n_proxy]) #CHECK

        # Если капча
        while links == 'Error':
            time.sleep(5)
            n_proxy = (n_proxy+1)%len(proxies)
            headers = {'accept-encoding': 'gzip, deflate', 'user-agent': generate_user_agent(), 'connection': 'keep-alive', 'accept': '*/*'}
            links = get_links(url, headers, proxies[n_proxy])
            print('Changing proxy', end = '')
            print(proxies[n_proxy])

        for idx, key in enumerate(links.keys()):
            print('Собираем информацию о модели: {}'.format(key), '({}/200)'.format(i*10+idx-9))
            item_dict = get_spec(links[key], headers, proxies[n_proxy])

            # Если капча
            while item_dict == 'Error':
                time.sleep(5)
                n_proxy = (n_proxy+1)%len(proxies)
                headers = {'accept-encoding': 'gzip, deflate', 'user-agent': generate_user_agent(), 'connection': 'keep-alive', 'accept': '*/*'}
                item_dict = get_spec(url, headers, proxies[n_proxy])
                print('Changing proxy')
                print(proxies[n_proxy])

            for key in item_dict.keys():
                specs_dict[key].append(int(item_dict[key]))
            time.sleep(random.randint(5,11))
            print('Done')

    # Посчитаем средние значения
    for key in specs_dict.keys():
        specs_dict[key] = sum(specs_dict[key])/len(specs_dict[key])

    # Сохраняем в файл
    to_file(specs_dict)