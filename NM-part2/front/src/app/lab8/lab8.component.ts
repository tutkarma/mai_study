import { Component, OnInit } from '@angular/core';
import { ApiClientService } from '../api-client.service';

declare var Plotly: any;

@Component({
  selector: 'app-lab8',
  templateUrl: './lab8.component.html',
  styleUrls: ['./lab8.component.css']
})
export class Lab8Component implements OnInit {

  method = 'alter_directions';
  dataNumerical: any
  dataAnalytic: any
  dataAnalyticX: any
  dataAnalyticY: any

  constructor(private apiClient: ApiClientService) {
  }

    solve(N1, N2, K, T) {
      const data = {
          N1: N1,
          N2: N2,
          K: K,
          T: T,
          equation_type: this.method
      };
      this.apiClient.executeSolver(8, data)
                    .subscribe(data => {
                      console.log(data);
                      this.dataNumerical = data['numerical'];
                      this.dataAnalytic = data['analytic']['grid'];
                      this.dataAnalyticX = data['analytic']['grid_x'];
                      this.dataAnalyticY = data['analytic']['grid_y'];
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

      const dataTraceX = {
        y: this.dataAnalyticX.map(s => this.sum(s) / s.length),
        type: 'scatter',
        name: 'Analytic X'
      };

      const dataTraceY = {
        y: this.dataAnalyticY.map(s => this.sum(s) / s.length),
        type: 'scatter',
        name: 'Analytic Y'
      };

      Plotly.newPlot('solution-comparison', [dataTraceX, dataTraceY]);
  }

  sum = arr => arr.reduce((a, b) => a + b, 0);

  ngOnInit(): void {
  }

}
