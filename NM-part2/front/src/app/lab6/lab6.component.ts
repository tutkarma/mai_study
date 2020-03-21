import { Component, OnInit } from '@angular/core';
import { ApiClientService } from '../api-client.service';

declare var Plotly: any;

@Component({
  selector: 'app-lab6',
  templateUrl: './lab6.component.html',
  styleUrls: ['./lab6.component.css']
})
export class Lab6Component implements OnInit {

  method = 'implicit';
  dataNumerical: any
  dataAnalytic: any

  constructor(private apiClient: ApiClientService) {
  }

  solve(N, K, T) {
      const data = {
          N: N,
          K: K,
          T: T,
          equation_type: this.method
      };
      this.apiClient.executeSolver(6, data)
                    .subscribe(data => {
                      console.log(data);
                      this.dataNumerical = data['numerical'];
                      this.dataAnalytic = data['analytic'];
      });

      const layoutNumerical = {
          title: this.method,
          autosize: false,
          width: 400,
          height: 400,
          margin: {
            l: 65,
            r: 50,
            b: 65,
            t: 90,
          }
      };
      const dataPlotNumerical = [{
          z: this.dataNumerical,
          type: 'surface'
      }];
      Plotly.newPlot('plotNumerical', dataPlotNumerical, layoutNumerical);

      const layoutAnalytic = {
          title: 'analytic',
          autosize: false,
          width: 400,
          height: 400,
          margin: {
            l: 65,
            r: 50,
            b: 65,
            t: 90,
          }
      };
      const dataPlotAnalytic = [{
          z: this.dataAnalytic,
          type: 'surface'
      }];
      Plotly.newPlot('plotAnalytic', dataPlotAnalytic, layoutAnalytic);

      const traceNumerical = {
        y: this.dataNumerical.map(s => this.sum(s) / s.length),
        type: 'scatter',
        name: this.method
      };

      const traceAnalytic = {
        y: this.dataAnalytic.map(s => this.sum(s) / s.length),
        type: 'scatter',
        name: 'analytic'
      };

      Plotly.newPlot('solution-comparison', [traceNumerical, traceAnalytic]);
  }

  sum = arr => arr.reduce((a, b) => a + b, 0);

  ngOnInit(): void {
  }

}
