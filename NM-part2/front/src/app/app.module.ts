import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { HttpClientModule } from '@angular/common/http';
import { MatSidenavModule } from '@angular/material/sidenav';
import { MatCardModule } from '@angular/material/card';
import { MatListModule } from '@angular/material/list';
import { MatSelectModule } from '@angular/material/select';
import { MatToolbarModule } from '@angular/material/toolbar';
import { MatIconModule } from '@angular/material/icon'; 

import { MathJaxModule } from 'ngx-mathjax';

import { AppRoutingModule } from './app-routing.module';
import { AppComponent } from './app.component';
import { Lab5Component } from './lab5/lab5.component';
import { BrowserAnimationsModule } from '@angular/platform-browser/animations';
import { Lab6Component } from './lab6/lab6.component';
import { Lab7Component } from './lab7/lab7.component';
import { Lab8Component } from './lab8/lab8.component';

@NgModule({
  declarations: [
    AppComponent,
    Lab5Component,
    Lab6Component,
    Lab7Component,
    Lab8Component
  ],
  imports: [
    BrowserModule,
    AppRoutingModule,
    HttpClientModule,
    BrowserAnimationsModule,
    MatSidenavModule,
    MatListModule,
    MathJaxModule.forRoot({
      version: '2.7.5',
      config: 'TeX-AMS_HTML',
      hostname: 'cdnjs.cloudflare.com'
    }),
    MatCardModule,
    MatSelectModule,
    MatToolbarModule,
    MatIconModule
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
