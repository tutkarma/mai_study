FROM python:3.7 as builder

RUN python3.7 -m venv /usr/share/python3/app
ENV PATH="/usr/share/python3/app/bin:${PATH}"

ADD requirements*.txt /tmp/
RUN pip install --no-cache-dir -Ur /tmp/requirements.txt

FROM builder as app

ADD solver/ /mnt/solver/
ADD app.py /mnt/solver/

SHELL ["/bin/bash", "-c"]
WORKDIR /mnt

EXPOSE 8080

CMD ["python3", "app.py"]