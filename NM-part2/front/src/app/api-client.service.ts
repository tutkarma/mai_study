import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';


@Injectable({
  providedIn: 'root'
})
export class ApiClientService {

  constructor(private http: HttpClient) {
  }

  public executeSolver(lab_id, data) {
    const url = `http://localhost:8080/labs/${lab_id}`

    const httpOptions = {
        headers: new HttpHeaders({
            'Content-Type':  'application/json',
            'Access-Control-Allow-Origin': '*'
        })
    };

    return this.http.post(url, data, httpOptions);
   }
}
